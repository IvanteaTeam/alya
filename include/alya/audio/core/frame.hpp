#pragma once
#include<alya/audio/core/sample.hpp>
#include<array>

namespace alya::audio::core
{

	template<typename WaveFormat>
	using frame = std::array<typename WaveFormat::sample_type, WaveFormat::channels>;

}
