#pragma once
#include<alya/audio/core/context_base.hpp>
#include<alya/audio/core/wave_format.hpp>
#include<functional>
#include<alya/audio/core/basic_buffer.hpp>

namespace alya::audio::core
{

	template<typename WaveFormat>
	class basic_context : public context_base
	{
	public:

		using wave_format = WaveFormat;
		using sample_type = typename wave_format::sample_type;

		static const size_t channels = wave_format::channels;

		explicit basic_context(device& d, size_t sample_rate)
			: context_base(d, details::make_sample_type<sample_type>(), channels, sample_rate)
		{}

		void set_fill_buffer_callback(std::function<void(basic_buffer<wave_format>)> callback)
		{
			fill_buffer_callback_ = callback;
		}

		void set_device_lost_callback(std::function<void()>callback)
		{
			device_lost_callback_ = callback;
		}

	private:

		virtual void fill_buffer(void*data, size_t frames)noexcept override
		{
			if(fill_buffer_callback_)
				fill_buffer_callback_({ reinterpret_cast<sample_type*>(data), frames });
		}

		virtual void handle_device_lost()noexcept
		{
			if(device_lost_callback_)
				device_lost_callback_();
		}
		std::function<void()> device_lost_callback_;
		std::function<void(basic_buffer<wave_format>)> fill_buffer_callback_;
	};

}
