#include<alya/audio/core/details/impl/wasapi_audio_client.hpp>
#include<alya/audio/core/device_lost.hpp>
#include<alya/audio/core/bad_wave_format.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<audioclient.h>
#include<mmdeviceapi.h>

namespace alya::audio::core::details
{

	wasapi_audio_client::wasapi_audio_client(const wasapi_device&device, sample_type sample, size_t channels, size_t sample_rate) : 
		write_event_handle_(nullptr, [](HANDLE handle)noexcept{ CloseHandle(handle); }),
		device_(device.device_)
	{
		activate_device();
		initialize(sample, channels, sample_rate);
		get_write_event_handle();
	}

	void wasapi_audio_client::activate_device()
	{
		auto res = device_->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&audio_client_));

		if (res != S_OK)
		{
			if (res == E_OUTOFMEMORY)
				throw std::bad_alloc{};
			else if (res == AUDCLNT_E_DEVICE_INVALIDATED)
				throw device_lost{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}
	}

	void wasapi_audio_client::initialize(sample_type sample, size_t channels, size_t sample_rate)
	{
		WAVEFORMATEX format;
		format.wFormatTag = details::is_sample_floating_point(sample) ? WAVE_FORMAT_IEEE_FLOAT : WAVE_FORMAT_PCM;
		format.wBitsPerSample = 8 * details::size_of_sample(sample);
		format.nSamplesPerSec = sample_rate;
		format.nChannels = channels;
		format.nBlockAlign = channels * details::size_of_sample(sample);
		format.nAvgBytesPerSec = channels * details::size_of_sample(sample) * sample_rate;
		format.cbSize = 0;

		auto res = audio_client_->Initialize(
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
	}

	void wasapi_audio_client::get_write_event_handle()
	{
		write_event_handle_.reset(CreateEvent(nullptr, false, false, nullptr));

		auto res = audio_client_->SetEventHandle(write_event_handle_.get());

		if (res != S_OK)
		{
			if (res == AUDCLNT_E_DEVICE_INVALIDATED)
				throw device_lost{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}
	}

	void wasapi_audio_client::start()
	{
		auto res = audio_client_->Start();
		if (res != S_OK)
		{
			if (res == AUDCLNT_E_DEVICE_INVALIDATED)
				throw device_lost{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}
	}

	void wasapi_audio_client::stop()noexcept
	{
		audio_client_->Stop();
	}

	void wasapi_audio_client::wait()
	{
		auto res = WaitForSingleObject(write_event_handle_.get(), INFINITE);
		if (res != WAIT_OBJECT_0)
			throw device_lost{};
	}

	size_t wasapi_audio_client::get_buffer_size()const
	{
		uint32_t size;
		auto res = audio_client_->GetBufferSize(&size);
		if (res != S_OK)
			throw device_lost{};
		return size;
	}

	size_t wasapi_audio_client::get_current_offset()const
	{
		uint32_t offset;
		auto res = audio_client_->GetCurrentPadding(&offset);
		if (res != S_OK)
			throw device_lost{};
		return offset;
	}

}
