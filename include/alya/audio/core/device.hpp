#pragma once
//#include<alya/utility/windows/com/shared_ptr.hpp>
//#include<alya/audio/core/details/wasapi_fwd.hpp>
#include<alya/audio/core/device_id.hpp>
#include<alya/audio/core/details/impl/wasapi_device.hpp>


namespace alya::audio::core
{

	class device
	{
	public:

		std::string_view name()const noexcept
		{
			return impl_.name();
		}
		bool is_valid()const noexcept
		{
			return impl_.is_valid();
		}
		device_id get_id()const
		{
			return impl_.get_id();
		}

	private:

		void get_device_name();
		
		explicit device(details::wasapi_device&&device) : impl_(std::move(device)) {}

		details::wasapi_device impl_;
		friend class context_base;
		friend class device_enumerator;
		friend class system_endpoint_base;
	};

}
