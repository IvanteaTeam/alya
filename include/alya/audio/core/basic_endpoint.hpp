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

	private:
		/*
		virtual void fill_buffer(sample_type*, size_t frames)noexcept = 0;

		virtual void fill_buffer(void*buffer, size_t frames)noexcept override
		{
			fill_buffer(static_cast<sample_type*>(buffer), frames);
		}

		virtual void handle_device_lost()noexcept override = 0;
		*/
	};

}
