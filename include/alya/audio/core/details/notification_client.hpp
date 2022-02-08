#pragma once
#include<alya/audio/core/details/device_list_event_type.hpp>
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<functional>
#include<mmdeviceapi.h>

namespace alya::audio::core::details
{

	class notification_client : IMMNotificationClient
	{
	public:
		notification_client(
			windows::com::shared_ptr<IMMDeviceEnumerator>device_enumerator,
			details::device_list_event_type event_type,
			std::function<void()> callback
		) :
			event_type_(event_type),
			device_enumerator_(device_enumerator),
			callback_(callback)
		{
			device_enumerator_->RegisterEndpointNotificationCallback(this);
		}

		~notification_client()
		{
			device_enumerator_->UnregisterEndpointNotificationCallback(this);
		}

		HRESULT __stdcall OnDefaultDeviceChanged(EDataFlow flow, ERole role, const wchar_t* device_id)override
		{
			if (role == ERole::eConsole && flow == eRender && event_type_ == details::device_list_event_type::default_device_changed)
				handle();
			return S_OK;
		}

		HRESULT __stdcall OnDeviceAdded(LPCWSTR device_id)override
		{
			if (event_type_ == details::device_list_event_type::device_list_changed)
				handle();
			return S_OK;
		}
		HRESULT __stdcall OnDeviceRemoved([[maybe_unused]] LPCWSTR device_id)override
		{
			if (event_type_ == details::device_list_event_type::device_list_changed)
				handle();
			return S_OK;
		}
		HRESULT __stdcall OnDeviceStateChanged([[maybe_unused]] LPCWSTR device_id, [[maybe_unused]] DWORD new_state)override
		{
			if (event_type_ == details::device_list_event_type::device_list_changed)
				handle();
			return S_OK;
		}
		HRESULT __stdcall OnPropertyValueChanged([[maybe_unused]] LPCWSTR device_id, [[maybe_unused]] const PROPERTYKEY key)override
		{
			return S_OK;
		}

		HRESULT __stdcall QueryInterface(const GUID& riid, void** requested_interface)
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

		ULONG __stdcall AddRef()
		{
			return 1;
		}

		ULONG __stdcall Release()
		{
			return 0;
		}

	private:

		void handle()noexcept
		{
			try
			{
				callback_();
			}
			catch (...)
			{

			}
		}

		details::device_list_event_type event_type_;
		windows::com::shared_ptr<IMMDeviceEnumerator> device_enumerator_;
		std::function<void()> callback_;
	};

}
