#include<alya/audio/core/details/impl/wasapi_clock_adjustment.hpp>
#include<alya/audio/core/device_lost.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<audioclient.h>

namespace alya::audio::core::details
{

	wasapi_clock_adjustment::wasapi_clock_adjustment(wasapi_audio_client& audio_client)
	{
		auto res = audio_client.audio_client_->GetService(__uuidof(IAudioClockAdjustment), reinterpret_cast<void**>(&clock_adjustment_));

		if (res != S_OK)
		{
			if (res == AUDCLNT_E_DEVICE_INVALIDATED)
				throw device_lost{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}
	}

	void wasapi_clock_adjustment::set_sample_rate(size_t sr)
	{
		clock_adjustment_->SetSampleRate(sr);
	}

}
