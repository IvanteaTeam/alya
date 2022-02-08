#include<alya/audio/core/device_enumerator.hpp>
#include<alya/utility/windows/com/create_instance.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<alya/audio/core/details/notification_client.hpp>

namespace alya::audio::core
{

	device_enumerator::device_enumerator()noexcept
	{
		try {
			device_enumerator_ = windows::com::create_instance<IMMDeviceEnumerator>(__uuidof(MMDeviceEnumerator));
		}
		catch (const std::system_error& e)
		{
			
		}
	}

	std::optional<device> device_enumerator::get_default_device()const noexcept
	{
		if (!device_enumerator_)
			return std::nullopt;

		try {
			windows::com::shared_ptr<IMMDevice> d;

			auto res = device_enumerator_->GetDefaultAudioEndpoint(eRender, eConsole, &d);

			if (res != S_OK)
				throw std::system_error{ windows::make_error_code(res) };
			
			return device(d);
		}
		catch (const std::exception& e)
		{
			return std::nullopt;
		}
	}

	std::vector<device> device_enumerator::get_devices()const noexcept
	{
		if (!device_enumerator_)
			return {};
		windows::com::shared_ptr<IMMDeviceCollection> collection;
		auto res = device_enumerator_->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &collection);
		if (res != S_OK)
			return {};

		uint32_t count;
		collection->GetCount(&count);

		std::vector<device> devices;

		try {
			devices.reserve(count);
			for (size_t i = 0; i < count; i++)
			{
				windows::com::shared_ptr<IMMDevice> d;
				collection->Item(i, &d);
				devices.push_back(device{ d });
			}
		}
		catch (const std::exception& e)
		{
			return {};
		}

		return devices;
	}

	void device_enumerator::set_device_list_callback(std::function<void()>callback, device_list_changed_t)
	{
		if (device_enumerator_)
			device_list_changed_notification_client_ = std::make_shared<details::notification_client>(device_enumerator_, details::device_list_event_type::device_list_changed, callback);
	}

	void device_enumerator::set_device_list_callback(std::function<void()>callback, default_device_changed_t)
	{
		if (device_enumerator_)
			default_device_changed_notification_client_ = std::make_shared<details::notification_client>(device_enumerator_, details::device_list_event_type::default_device_changed, callback);
	}
	
}
