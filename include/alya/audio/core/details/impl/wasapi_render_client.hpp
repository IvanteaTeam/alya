#pragma once
#include<alya/audio/core/details/impl/wasapi_audio_client.hpp>

struct IAudioRenderClient;

namespace alya::audio::core::details
{

	class wasapi_render_client
	{
	public:

		explicit wasapi_render_client(wasapi_audio_client&);
		wasapi_render_client(const wasapi_render_client&) = delete;
		wasapi_render_client(wasapi_render_client&&) = default;
		wasapi_render_client& operator=(const wasapi_render_client&) = delete;
		wasapi_render_client& operator=(wasapi_render_client&&) = default;

		struct buffer_releaser
		{
			windows::com::shared_ptr<IAudioRenderClient> render_client;
			size_t size;
			void operator()(void*)noexcept;
		};

		std::unique_ptr<void, buffer_releaser> get_buffer(size_t frames);

	private:
		windows::com::shared_ptr<IAudioRenderClient> render_client_;
	};

}
