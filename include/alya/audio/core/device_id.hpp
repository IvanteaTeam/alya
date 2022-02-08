#pragma once
#include<string>

namespace alya::audio::core
{
	
	class device_id
	{
	public:

		device_id() = default;

		friend bool operator==(const device_id&a, const device_id&b)noexcept
		{
			return a.id_ == b.id_;
		}

		friend bool operator!=(const device_id& a, const device_id& b)noexcept
		{
			return a.id_ != b.id_;
		}

		operator bool()const noexcept
		{
			return *this == device_id{};
		}

	private:
		
		explicit device_id(std::wstring id)
			: id_(id)
		{}

		std::wstring id_;
		friend class device_enumerator;
		friend class device;
	};

}
