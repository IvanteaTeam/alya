#pragma once
#include<alya/audio/core/buffer.hpp>

namespace alya::audio
{

	template<typename WaveFormat>
	class basic_channel
	{
	public:
		using wave_format = WaveFormat;
		using frame_type = core::frame<wave_format>;
		
		virtual size_t sample_rate()const noexcept = 0;
		virtual core::buffer<wave_format> get_data(size_t frames)noexcept = 0;
		virtual ~basic_channel() = default;
		
	};

}
