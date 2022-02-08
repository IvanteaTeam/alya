#pragma once
#include<alya/audio/core/device.hpp>
#include<alya/utility/unique_handle.hpp>
#include<alya/audio/core/details/sample_type.hpp>
#include<atomic>
#include<thread>

namespace alya::audio::core
{

	class context_base
	{
	public:

		context_base(device&, details::sample_type, size_t channels, size_t sample_rate);
		context_base(const context_base&) = delete;
		context_base(context_base&&) = delete;
		~context_base();
		context_base& operator=(const context_base&) = delete;
		context_base& operator=(context_base&&) = delete;
		
		void start();
		void stop()noexcept;
		bool is_running()const noexcept;
		bool is_device_valid()const noexcept;
		void set_sample_rate(size_t);
		size_t get_sample_rate()const noexcept;

	private:

		virtual void fill_buffer(void*, size_t)noexcept = 0;
		virtual void handle_device_lost()noexcept = 0;

		void start_audio_thread();
		void stop_audio_thread();

		mutable windows::com::shared_ptr<IMMDevice> device_;
		windows::com::shared_ptr<IAudioClient> audio_client_;
		windows::com::shared_ptr<IAudioRenderClient> audio_render_client_;
		windows::com::shared_ptr<IAudioClockAdjustment> audio_clock_adjustemnt_;
		unique_handle<void*, void(*)(void*)> event_handle_;
		std::atomic<bool> is_running_;
		std::thread audio_thread_;
		size_t sample_rate_;
	};

}
