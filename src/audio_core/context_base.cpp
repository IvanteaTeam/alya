#include<alya/audio/core/context_base.hpp>
#include<alya/utility/windows/com/create_instance.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<alya/audio/core/device_lost.hpp>
#include<alya/audio/core/bad_wave_format.hpp>
#include<audioclient.h>
#include<mmdeviceapi.h>

namespace alya::audio::core
{
	
	context_base::context_base(device& d, details::sample_type sample_type, size_t channels, size_t sample_rate) :
		event_handle_(nullptr, [](HANDLE h) { if (h) CloseHandle(h); }),
		device_(d.device_),
		is_running_(false),
		audio_clock_adjustemnt_(nullptr),
		sample_rate_(sample_rate)
	{

		auto res = d.device_->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&audio_client_));

		if (res != S_OK)
		{
			if (res == E_OUTOFMEMORY)
				throw std::bad_alloc{};
			else if (res == AUDCLNT_E_DEVICE_INVALIDATED)
				throw device_lost{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}

		WAVEFORMATEX format;
		format.wFormatTag = details::is_sample_floating_point(sample_type) ? WAVE_FORMAT_IEEE_FLOAT : WAVE_FORMAT_PCM;
		format.wBitsPerSample = 8 * details::size_of_sample(sample_type);
		format.nSamplesPerSec = sample_rate_;
		format.nChannels = channels;
		format.nBlockAlign = channels * details::size_of_sample(sample_type);
		format.nAvgBytesPerSec = channels * details::size_of_sample(sample_type) * sample_rate; 
		format.cbSize = 0;

		res = audio_client_->Initialize(
			AUDCLNT_SHAREMODE_SHARED,
			AUDCLNT_STREAMFLAGS_RATEADJUST | AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
			0,
			0,
			&format,
			nullptr
		);

		if (res != S_OK)
		{
			if (res == AUDCLNT_E_DEVICE_INVALIDATED || res == AUDCLNT_E_DEVICE_IN_USE || res == AUDCLNT_E_ENDPOINT_CREATE_FAILED)
				throw device_lost{};
			else if (res == E_OUTOFMEMORY)
				throw std::bad_alloc{};
			else if (res == AUDCLNT_E_UNSUPPORTED_FORMAT)
				throw bad_wave_format{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}

		res = audio_client_->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&audio_render_client_));

		if (res != S_OK)
		{
			if (res == AUDCLNT_E_DEVICE_INVALIDATED)
				throw device_lost{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}

		res = audio_client_->GetService(__uuidof(IAudioClockAdjustment), reinterpret_cast<void**>(&audio_clock_adjustemnt_));

		if (res != S_OK)
		{
			if (res == AUDCLNT_E_DEVICE_INVALIDATED)
				throw device_lost{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}

		event_handle_.reset(CreateEvent(nullptr, false, false, nullptr));

		res = audio_client_->SetEventHandle(event_handle_.value());

		if (res != S_OK)
		{
			if (res == AUDCLNT_E_DEVICE_INVALIDATED)
				throw device_lost{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}

	}

	context_base::~context_base()
	{
		stop();
	}

	bool context_base::is_running()const noexcept
	{
		return is_running_;
	}

	bool context_base::is_device_valid()const noexcept
	{
		DWORD state;
		device_->GetState(&state);
		return state == DEVICE_STATE_ACTIVE;
	}

	void context_base::start()
	{
		if (!is_running_)
		{
			
			auto res = audio_client_->Start();
			if (res != S_OK)
			{
				if (res == AUDCLNT_E_DEVICE_INVALIDATED)
					throw device_lost{};
				else
					throw std::system_error{ windows::make_error_code(res) };
			}
			start_audio_thread();
		}
	}

	void context_base::start_audio_thread()
	{
		is_running_ = true;
		audio_thread_ = std::thread{ [this]() {

			uint32_t frames;

			auto res = audio_client_->GetBufferSize(&frames);
			
			if (res == S_OK)
			{

				while (is_running_)
				{
					uint32_t curr;
					res = audio_client_->GetCurrentPadding(&curr);
					if (res != S_OK)
						break;

					uint8_t* ptr;

					res = audio_render_client_->GetBuffer(frames - curr, &ptr);
					if (res != S_OK)
						break;

					fill_buffer(ptr, frames - curr);
					res = audio_render_client_->ReleaseBuffer(frames - curr, 0);
					if (res != S_OK)
						break;
					res = WaitForSingleObject(event_handle_, INFINITE);
					if (res != WAIT_OBJECT_0)
						break;
				}
			}

			if (res != S_OK || res != WAIT_OBJECT_0)
			{
				handle_device_lost();
			}
		} };
	}

	void context_base::stop_audio_thread()
	{
		is_running_ = false;
		if (std::this_thread::get_id() == audio_thread_.get_id())
			audio_thread_.detach();
		else
			audio_thread_.join();
	}

	void context_base::stop()noexcept
	{
		if (is_running_)
		{
			stop_audio_thread();
			audio_client_->Stop();
		}
	}

	void context_base::set_sample_rate(size_t rate)
	{
		auto res = audio_clock_adjustemnt_->SetSampleRate(rate);
		if (res != S_OK)
			throw std::out_of_range{"Sample rate is out of range"};
	}

	size_t context_base::get_sample_rate()const noexcept
	{
		return sample_rate_;
	}

}
