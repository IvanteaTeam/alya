#include<alya/audio/core/details/impl/wasapi_device_enumerator.hpp>
#include<alya/utility/windows/com/create_instance.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<alya/audio/core/details/impl/wasapi_notification_client.hpp>

namespace alya::audio::core::details
{

	wasapi_device_enumerator::wasapi_device_enumerator()noexcept
	{
		try {
			enumerator_ = windows::com::create_instance<IMMDeviceEnumerator>(__uuidof(MMDeviceEnumerator));
		}
		catch (const std::system_error& e)
		{

		}
	}

	std::optional<wasapi_device> wasapi_device_enumerator::get_default_device()const
	{
		if (!enumerator_)
			return std::nullopt;

		try {
			windows::com::shared_ptr<IMMDevice> d;

			auto res = enumerator_->GetDefaultAudioEndpoint(eRender, eConsole, &d);

			if (res != S_OK)
				throw std::system_error{ windows::make_error_code(res) };

			return wasapi_device(d);
		}
		catch (const std::exception& e)
		{
			return std::nullopt;
		}
	}

	void wasapi_device_enumerator::set_event_callback(std::function<void()>callback, device_list_event_type event_type)
	{
		using enum device_list_event_type;
		if (event_type == default_device_changed)
			default_device_change_ = std::make_shared<wasapi_notification_client>(enumerator_, event_type, std::move(callback));
		else if(event_type == device_list_changed)
			device_list_change_ = std::make_shared<wasapi_notification_client>(enumerator_, event_type, std::move(callback));
	}

}
