#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<alya/audio/core/details/wasapi_fwd.hpp>
#include<alya/audio/core/device_id.hpp>

namespace alya::audio::core
{

	class device
	{
	public:

		device() = delete;
		explicit device(const device_id&);
		device(const device&) = default;
		device(device&&) = default;
		device& operator=(const device&) = default;
		device& operator=(device&&) = default;

		std::string_view name()const noexcept;
		bool is_valid()const noexcept;
		device_id get_id()const;

	private:

		void get_device_name();

		explicit device(windows::com::shared_ptr<IMMDevice>);
		mutable windows::com::shared_ptr<IMMDevice> device_;
		std::string name_;
		std::wstring id_;
		friend class context_base;
		friend class device_enumerator;
	};

}
