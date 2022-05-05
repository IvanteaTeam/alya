#pragma once
#include<alya/audio/core/details/device_list_event_type.hpp>
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<functional>
#include<mmdeviceapi.h>

namespace alya::audio::core::details
{

	class wasapi_notification_client : public IMMNotificationClient
	{
	public:

		wasapi_notification_client(
			windows::com::shared_ptr<IMMDeviceEnumerator> enumerator,
			details::device_list_event_type event_type,
			std::function<void()> callback
		) :
			event_type_(event_type),
			enumerator_(enumerator),
			callback_(callback)
		{
			enumerator_->RegisterEndpointNotificationCallback(this);
		}

		~wasapi_notification_client()
		{
			enumerator_->UnregisterEndpointNotificationCallback(this);
		}

		long __stdcall OnDefaultDeviceChanged(EDataFlow flow, ERole role, const wchar_t* device_id)override
		{
			if (role == ERole::eConsole && flow == eRender)
				handle_default_device_change();
			return S_OK;
		}

		long __stdcall OnDeviceAdded(LPCWSTR device_id)override
		{
			handle_device_list_change();
			return S_OK;
		}
		long __stdcall OnDeviceRemoved(const wchar_t* device_id)override
		{
			handle_device_list_change();
			return S_OK;
		}
		long __stdcall OnDeviceStateChanged(const wchar_t* device_id, unsigned long new_state)override
		{
			handle_device_list_change();
			return S_OK;
		}
		long __stdcall OnPropertyValueChanged(const wchar_t* device_id, const PROPERTYKEY key)override
		{
			return S_OK;
		}

		long __stdcall QueryInterface(const GUID& riid, void** requested_interface)override
		{
			if (IID_IUnknown == riid)
			{
				*requested_interface = static_cast<IUnknown*>(this);
			}
			else if (__uuidof(IMMNotificationClient) == riid)
			{
				*requested_interface = static_cast<IMMNotificationClient*>(this);
			}
			else
			{
				*requested_interface = nullptr;
				return E_NOINTERFACE;
			}
			return S_OK;
		}

		unsigned long __stdcall AddRef()override
		{
			return 1;
		}

		unsigned long __stdcall Release()override
		{
			return 0;
		}

	private:

		void handle_default_device_change()noexcept
		{
			if (event_type_ == device_list_event_type::default_device_changed)
				do_handle();
		}
		void handle_device_list_change()noexcept
		{
			if (event_type_ == device_list_event_type::device_list_changed)
				do_handle();
		}

		void do_handle()noexcept
		{
			try
			{
				callback_();
			}
			catch (...)
			{

			}
		}
		device_list_event_type event_type_;
		std::function<void()> callback_;
		windows::com::shared_ptr<IMMDeviceEnumerator> enumerator_;
	};

}
