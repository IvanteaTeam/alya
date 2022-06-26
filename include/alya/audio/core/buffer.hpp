#pragma once
#include<alya/audio/core/frame.hpp>
#include<span>

namespace alya::audio::core
{

	template<typename WaveFormat>
	using buffer = std::span<frame<WaveFormat>>;

}
