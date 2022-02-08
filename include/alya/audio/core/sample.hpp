#pragma once
#include<alya/audio/core/details/sample_type.hpp>

namespace alya::audio::core
{
	
	template<typename T>
	concept sample = std::same_as<T, int16n_t> || std::same_as<T, int32n_t> || std::same_as<T, float>;

}
