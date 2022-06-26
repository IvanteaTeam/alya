#pragma once
#include<alya/audio/core/system_endpoint_base.hpp>
#include<alya/audio/core/basic_endpoint.hpp>

namespace alya::audio::core
{

	template<typename WaveFormat>
	class basic_system_endpoint : public system_endpoint_base, public basic_endpoint<WaveFormat>
	{
	public:

		using wave_format = WaveFormat;
		using sample_type = typename wave_format::sample_type;
		static constexpr size_t channels = wave_format::channels;

		basic_system_endpoint(const device& device_, size_t sample_rate) :
			system_endpoint_base(device_, details::make_sample_type<sample_type>(), channels, sample_rate)
		{}

		void start() override
		{
			system_endpoint_base::start();
		}
		void stop()noexcept override
		{
			system_endpoint_base::stop();
		}
		bool is_started()const noexcept override
		{
			return system_endpoint_base::is_started();
		}
		bool is_valid()const noexcept override
		{
			return system_endpoint_base::is_valid();
		}
		void set_audio_callback(std::function<void(buffer<wave_format>)>cb) override
		{
			audio_callback_ = std::move(cb);
		}
	
		size_t sample_rate()const noexcept override
		{
			return system_endpoint_base::sample_rate();
		}

		void set_sample_rate(size_t sr) override
		{
			system_endpoint_base::set_sample_rate(sr);
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
