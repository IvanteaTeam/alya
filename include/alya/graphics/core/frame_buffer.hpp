#pragma once
#include<alya/graphics/core/basic_frame_buffer.hpp>

namespace alya::graphics::core
{

	using frame_buffer = basic_frame_buffer<1, false>;
	
	template<size_t S>
	using multisamled_frame_buffer = basic_frame_buffer<S, false>;

	using layered_frame_buffer = basic_frame_buffer<1, true>;

	template<size_t S>
	using muiltisampled_layered_frame_buffer = basic_frame_buffer<S, true>;

}
