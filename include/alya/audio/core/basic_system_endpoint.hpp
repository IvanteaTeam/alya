#pragma once
#include<alya/audio/core/system_endpoint_base.hpp>
#include<alya/audio/core/basic_endpoint.hpp>

namespace alya::audio::core
{

	template<typename WaveFormat>
	class basic_system_endpoint : public system_endpoint_base, basic_endpoint<WaveFormat>
	{
	public:

		using wave_format = WaveFormat;
		using sample_type = typename wave_format::sample_type;
		static constexpr size_t channels = wave_format::channels;

		basic_system_endpoint(const device& device_, size_t sample_rate) :
			system_endpoint_base(device_, details::make_sample_type<sample_type>(), channels, sample_rate)
		{}

		virtual void start() 
		{
			system_endpoint_base::start();
		}
		virtual void stop()noexcept 
		{
			system_endpoint_base::stop();
		}
		virtual bool is_started()const noexcept 
		{
			return system_endpoint_base::is_started();
		}
		virtual bool is_valid()const noexcept 
		{
			return system_endpoint_base::is_valid();
		}
		virtual void set_audio_callback(std::function<void(buffer<wave_format>)>cb)
		{
			audio_callback_ = std::move(cb);
		}

	private:

		virtual void fill_buffer(frame<wave_format>*buffer, size_t frames)noexcept 
		{
			if (audio_callback_)
				audio_callback_({ buffer, frames });
		}

		virtual void fill_buffer(void* buffer, size_t frames)noexcept override
		{
			fill_buffer(static_cast<frame<wave_format>*>(buffer), frames);
		}

		virtual void handle_device_lost()noexcept override {}
		std::function<void(buffer<wave_format>)> audio_callback_;
	};

}
