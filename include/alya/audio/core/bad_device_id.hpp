#pragma once
#include<stdexcept>

namespace alya::audio::core
{
		
	class bad_device_id : public std::runtime_error
	{
	public:
		explicit bad_device_id() : std::runtime_error("Invalid device identifier.") {}
	};

}
