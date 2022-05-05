#pragma once
#include<alya/audio/core/details/impl/wasapi_device.hpp>
#include<memory>
#include<alya/audio/core/details/device_list_event_type.hpp>
#include<vector>
#include<optional>
#include<functional>

struct IMMDeviceEnumerator;

namespace alya::audio::core::details
{

	class wasapi_notification_client;

	class wasapi_device_enumerator
	{
	public:
		
		wasapi_device_enumerator()noexcept;
		wasapi_device_enumerator(const wasapi_device_enumerator&) = delete;
		wasapi_device_enumerator(wasapi_device_enumerator&&) = delete;
		wasapi_device_enumerator& operator=(const wasapi_device_enumerator&) = delete;
		wasapi_device_enumerator& operator=(wasapi_device_enumerator&&) = delete;

		std::optional<wasapi_device> get_default_device()const;
		std::vector<wasapi_device> get_devices()const;

		void set_event_callback(std::function<void()>, device_list_event_type);

	private:
		mutable windows::com::shared_ptr<IMMDeviceEnumerator> enumerator_;
		std::shared_ptr<wasapi_notification_client> default_device_change_, device_list_change_;
	};

}
