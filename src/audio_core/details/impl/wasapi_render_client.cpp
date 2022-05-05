#include<alya/audio/core/details/impl/wasapi_render_client.hpp>
#include<alya/audio/core/device_lost.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<audioclient.h>

namespace alya::audio::core::details
{

	wasapi_render_client::wasapi_render_client(wasapi_audio_client&audio_client)
	{
		auto res = audio_client.audio_client_->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&render_client_));

		if (res != S_OK)
		{
			if (res == AUDCLNT_E_DEVICE_INVALIDATED)
				throw device_lost{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}
	}

	std::unique_ptr<void, wasapi_render_client::buffer_releaser> wasapi_render_client::get_buffer(size_t size)
	{
		uint8_t* ptr;
		auto res = render_client_->GetBuffer(size, &ptr);
		if (res != S_OK)
			throw device_lost{};
		return { ptr, buffer_releaser{render_client_, size} };
	}

	void wasapi_render_client::buffer_releaser::operator()(void*)noexcept
	{
		render_client->ReleaseBuffer(size, 0);
	}

}
