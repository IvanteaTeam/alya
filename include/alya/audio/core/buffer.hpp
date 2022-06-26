#pragma once
#include<alya/audio/core/frame.hpp>
#include<span>

namespace alya::audio::core
{

	template<typename WaveFormat>
	using buffer = std::span<frame<WaveFormat>>;

	template<typename WaveFormat>
	using const_buffer = std::span<const frame<WaveFormat>>;

}
