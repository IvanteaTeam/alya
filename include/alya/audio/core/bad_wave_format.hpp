#pragma once
#include<stdexcept>

namespace alya::audio::core
{

	class bad_wave_format : public std::runtime_error
	{
	public:
		explicit bad_wave_format() : runtime_error("Invalid wave format.") {}
	};

}
