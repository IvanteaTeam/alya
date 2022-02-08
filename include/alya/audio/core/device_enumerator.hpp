#pragma once
#include<alya/audio/core/device.hpp>
#include<alya/audio/core/device_list_events.hpp>
#include<functional>
#include<optional>

namespace alya::audio::core
{ 

	namespace details
	{

		class notification_client;

	}

	class device_enumerator
	{
	public:

		device_enumerator()noexcept;
		device_enumerator(const device_enumerator&) = delete;
		device_enumerator(device_enumerator&&) = default;
		device_enumerator& operator=(const device_enumerator&) = delete;
		device_enumerator& operator=(device_enumerator&&) = default;

		std::vector<device> get_devices()const noexcept;
		std::optional<device> get_default_device()const noexcept;
		
		void set_device_list_callback(std::function<void()>, default_device_changed_t);
		void set_device_list_callback(std::function<void()>, device_list_changed_t);

	private:

		std::shared_ptr<details::notification_client>
			default_device_changed_notification_client_,
			device_list_changed_notification_client_;

		mutable windows::com::shared_ptr<IMMDeviceEnumerator> device_enumerator_;
	};
	
}
