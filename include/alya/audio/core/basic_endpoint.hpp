#pragma once
#include<alya/audio/core/buffer.hpp>
#include<functional>

namespace alya::audio::core
{

	template<typename WaveFormat>
	class basic_endpoint
	{
	public:
		
		using wave_format = WaveFormat;
		using sample_type = typename wave_format::sample_type;
		static constexpr size_t channels = wave_format::channels;
		
		virtual void start() = 0;
		virtual void stop()noexcept = 0;
		virtual bool is_started()const noexcept = 0;
		virtual bool is_valid()const noexcept = 0;
		virtual void set_audio_callback(std::function<void(buffer<wave_format>)>) = 0;
		virtual void set_sample_rate(size_t) = 0;
		virtual size_t sample_rate()const noexcept = 0;
		virtual ~basic_endpoint() = default;
	};

}
