#pragma once
#include<alya/audio/core/details/impl/wasapi_audio_client.hpp>
#include<alya/audio/core/details/impl/wasapi_render_client.hpp>
#include<alya/audio/core/details/impl/wasapi_clock_adjustment.hpp>
#include<thread>

namespace alya::audio::core::details
{

	class wasapi_endpoint
	{
	public:

		wasapi_endpoint(const wasapi_device&device, sample_type sample, size_t channels, size_t sample_rate) :
			audio_client_(device, sample, channels, sample_rate), 
			render_client_(audio_client_),
			clock_adjustment_(audio_client_), 
			sample_rate_(sample_rate)
		{}

		bool is_running()const noexcept
		{
			return is_running_;
		}

		bool is_valid()const noexcept
		{
			return audio_client_.is_valid();
		}

		void start()
		{
			if (!is_running())
				do_start();
		}

		void stop()
		{
			if (is_running())
				do_stop();
		}

		void set_sample_rate(size_t sample_rate)
		{
			clock_adjustment_.set_sample_rate(sample_rate);
			sample_rate_ = sample_rate;
		}

	private:

		void do_start()
		{
			audio_client_.start();
			start_audio_thread();
		}

		void do_stop()noexcept
		{
			stop_audio_thread();
			audio_client_.stop();
		}

		void start_audio_thread()noexcept
		{
			is_running_ = true;
			audio_thread_ = std::thread{ [&]() {

				try
				{

					size_t buffer_size = audio_client_.get_buffer_size();

					while (is_running_)
					{
						{
							size_t offset = audio_client_.get_current_offset();
							auto buffer = render_client_.get_buffer(buffer_size - offset);
							fill_buffer(buffer.get(), buffer_size - offset);
						}
						audio_client_.wait();
					}

				}
				catch (...)
				{
					handle_device_lost();
				}
			} };

		}

		void stop_audio_thread()noexcept
		{
			is_running_ = false;
			if (std::this_thread::get_id() == audio_thread_.get_id())
				audio_thread_.detach();
			else
				audio_thread_.join();
		}

		virtual void fill_buffer(void*, size_t frames)noexcept = 0;
		virtual void handle_device_lost()noexcept = 0;

		std::atomic<bool> is_running_;
		size_t sample_rate_;
		wasapi_audio_client audio_client_;
		wasapi_render_client render_client_;
		wasapi_clock_adjustment clock_adjustment_;
		std::thread audio_thread_;
	};

}
