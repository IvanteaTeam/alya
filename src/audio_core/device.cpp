#include<alya/audio/core/device.hpp>
#include<alya/utility/windows/com/create_instance.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<alya/audio/core/bad_device_id.hpp>
#include<utf8.h>
#include <mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>

namespace alya::audio::core
{
	
	device::device(windows::com::shared_ptr<IMMDevice>d) : 
		device_(d)
	{

		get_device_name();
		
		if (name_ == "")
			throw std::exception{};

		wchar_t* id;

		auto res = device_->GetId(&id);

		if (res != S_OK)
		{
			if (res == E_OUTOFMEMORY)
				throw std::bad_alloc{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}

		try
		{
			id_ = id;
		}
		catch (const std::exception& e)
		{
			CoTaskMemFree(id);
			throw;
		}

		CoTaskMemFree(id);

	}

	device::device(const device_id& id)
	{
		auto device_enumerator = windows::com::create_instance<IMMDeviceEnumerator>(__uuidof(MMDeviceEnumerator));
		windows::com::shared_ptr<IMMDevice> d;
		auto res = device_enumerator->GetDevice(id.id_.c_str(), &d);
		if (res != S_OK)
		{
			if (res == E_OUTOFMEMORY)
				throw std::bad_alloc{};
			else if (res == E_NOTFOUND)
				throw bad_device_id{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}
		*this = device(d);
	}

	void device::get_device_name()
	{
		windows::com::shared_ptr<IPropertyStore> props;

		auto res = device_->OpenPropertyStore(STGM_READ, &props);

		if (res != S_OK)
		{
			if (res == E_OUTOFMEMORY)
				throw std::bad_alloc{};
			else
				throw std::system_error{ windows::make_error_code(res) };
		}

		PROPVARIANT name;
		PropVariantInit(&name);

		auto get_name = [&](auto& prop)
		{
			auto res = props->GetValue(prop, &name);
			return res == S_OK;
		};

		if (get_name(PKEY_Device_FriendlyName) || get_name(PKEY_DeviceInterface_FriendlyName) || get_name(PKEY_Device_DeviceDesc))
		{
			const wchar_t* ptr = name.pwszVal;
			
			try {
				utf8::utf16to8(ptr, ptr + std::wcslen(ptr), std::back_inserter(name_));
			}
			catch (const std::exception& e)
			{
				name_ = "";
			}
		}

		PropVariantClear(&name);
	}

	bool device::is_valid()const noexcept
	{
		DWORD state;
		device_->GetState(&state);
		return state == DEVICE_STATE_ACTIVE;
	}

	std::string_view device::name()const noexcept
	{
		return name_;
	}

	device_id device::get_id()const
	{
		return device_id{ id_ };
	}

}
