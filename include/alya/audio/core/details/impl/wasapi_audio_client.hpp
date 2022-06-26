#pragma once
#include<alya/audio/core/details/impl/wasapi_device.hpp>
#include<alya/audio/core/details/sample_type.hpp>
#include<memory>
#include<optional>

struct IAudioClient;

namespace alya::audio::core::details
{

	class wasapi_audio_client
	{
	public:

		wasapi_audio_client(const wasapi_device&, sample_type sample, size_t channels, size_t sample_rate);
		wasapi_audio_client(const wasapi_audio_client&) = delete;
		wasapi_audio_client(wasapi_audio_client&&) = default;
		wasapi_audio_client& operator=(const wasapi_audio_client&) = delete;
		wasapi_audio_client& operator=(wasapi_audio_client&&) = default;

		bool is_valid()const noexcept
		{
			return true;
		}
		void start();
		void stop()noexcept;

		void wait();
		size_t get_buffer_size()const;
		size_t get_current_offset()const;

	private:

		void activate_device();
		void initialize(sample_type sample, size_t channels, size_t sample_rate);
		void get_write_event_handle();

		mutable windows::com::shared_ptr<IAudioClient> audio_client_;
		mutable windows::com::shared_ptr<IMMDevice> device_;
		std::unique_ptr<void, void(*)(void*)noexcept> write_event_handle_;
		friend class wasapi_render_client;
		friend class wasapi_clock_adjustment;
	};

}
