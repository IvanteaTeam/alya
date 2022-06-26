#pragma once
#include<alya/audio/core/basic_endpoint.hpp>
#include<alya/audio/basic_channel.hpp>

namespace alya::audio
{

	template<typename WaveFormat>
	class basic_context
	{
	public:
	
		using wave_format = WaveFormat;
		using endpoint_type = core::basic_endpoint<wave_format>;
		using channel_type = basic_channel<wave_format>;

		basic_context(endpoint_type&ep) : endpoint_(ep), channel_(nullptr)
		{
			ep.set_audio_callback([this](auto buffer) {
				if (channel_ && buffer.size() > 0)
				{
					auto data = channel_->get_data(buffer.size());
					std::copy(data.begin(), data.end(), buffer.begin());
				}
			});
			ep.start();
		}

		void set_otput_channel(channel_type&ch)noexcept
		{
			endpoint_.stop();
			channel_ = &ch;
			try
			{
				endpoint_.set_sample_rate(channel_->sample_rate());
			}
			catch (...)
			{

			}
			endpoint_.start();
		}

	private:
		endpoint_type& endpoint_;
		channel_type* channel_;
	};

}
