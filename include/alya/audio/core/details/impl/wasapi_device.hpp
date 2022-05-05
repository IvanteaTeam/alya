#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<alya/audio/core/details/impl/wasapi_device_id.hpp>

struct IMMDevice;

namespace alya::audio::core::details
{

	class wasapi_device
	{
	public:

		wasapi_device() = delete;

		bool is_valid()const noexcept;
		wasapi_device_id get_id()const
		{
			return id_;
		}
		std::string_view name()const noexcept
		{
			return name_;
		}

	private:

		explicit wasapi_device(windows::com::shared_ptr<IMMDevice> device);

		mutable windows::com::shared_ptr<IMMDevice> device_;
		wasapi_device_id id_;
		std::string name_;
		friend class wasapi_device_enumerator;
		friend class wasapi_audio_client;
	};

}
