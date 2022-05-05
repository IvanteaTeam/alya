#pragma once
#include<alya/audio/core/details/impl/wasapi_audio_client.hpp>

struct IAudioClockAdjustment;

namespace alya::audio::core::details
{

	class wasapi_clock_adjustment
	{
	public:

		explicit wasapi_clock_adjustment(wasapi_audio_client&);
		wasapi_clock_adjustment(const wasapi_clock_adjustment&) = delete;
		wasapi_clock_adjustment(wasapi_clock_adjustment&&) = default;
		wasapi_clock_adjustment& operator=(const wasapi_clock_adjustment&) = delete;
		wasapi_clock_adjustment& operator=(wasapi_clock_adjustment&&) = default;

		void set_sample_rate(size_t);

	private:
		windows::com::shared_ptr<IAudioClockAdjustment> clock_adjustment_;
	};

}
