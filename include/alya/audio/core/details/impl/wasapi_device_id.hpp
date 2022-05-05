#pragma once 
#include<string>

namespace alya::audio::core::details
{

	class wasapi_device_id
	{
	public:

		wasapi_device_id() = default;

		bool operator==(const wasapi_device_id&) const = default;
		bool operator!=(const wasapi_device_id&) const = default;

		operator bool()const noexcept
		{
			return *this == wasapi_device_id{};
		}

	private:

		explicit wasapi_device_id(std::wstring id)
			: id_(id)
		{}

		std::wstring id_;
		friend class device_enumerator;
		friend class device;
	};

}
