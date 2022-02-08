#pragma once
#include<stdexcept>

namespace alya::audio::core
{

	class device_lost : public std::runtime_error 
	{
	public:
		explicit device_lost() : runtime_error("Audio device has been lost.") {};
	};

}
