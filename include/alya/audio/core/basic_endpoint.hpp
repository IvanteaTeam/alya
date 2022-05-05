#pragma once
#include<alya/audio/core/endpoint_base.hpp>

namespace alya::audio::core
{

	template<typename WaveFormat>
	class basic_endpoint : public endpoint_base
	{
	public:
		
		using wave_format = WaveFormat;
		using sample_type = typename wave_format::sample_type;
		static constexpr size_t channels = wave_format::channels;

		basic_endpoint(const device&device_, size_t sample_rate) : 
			endpoint_base(device_, details::make_sample_type<sample_type>(), channels, sample_rate)
		{}

	private:

		virtual void fill_buffer(sample_type*, size_t frames)noexcept = 0;

		virtual void fill_buffer(void*buffer, size_t frames)noexcept override
		{
			fill_buffer(static_cast<sample_type*>(buffer), frames);
		}

		virtual void handle_device_lost()noexcept override = 0;

	};

}
