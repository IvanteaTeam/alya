#pragma once
#include<alya/graphics/basic_color_buffer_view.hpp>

namespace alya::graphics
{

	using color_buffer_view = basic_color_buffer_view<1, false>;
	
	template<size_t S>
	using multisampled_color_buffer_view = basic_color_buffer_view<S, false>;

	using layered_color_buffer_view = basic_color_buffer_view<1, true>;

	template<size_t S>
	using multisampled_layered_color_buffer_view = basic_color_buffer_view<S, true>;

}
