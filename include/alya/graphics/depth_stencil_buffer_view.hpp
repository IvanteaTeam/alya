#pragma once
#include<alya/graphics/basic_depth_stencil_buffer_view.hpp>

namespace alya::graphics
{
	
	using depth_stencil_buffer_view = basic_depth_stencil_buffer_view<1, false>;

	template<size_t S>
	using multisampled_depth_stencil_buffer_view = basic_depth_stencil_buffer_view<S, false>;

	using layered_depth_stencil_buffer_view = basic_depth_stencil_buffer_view<1, true>;

	template<size_t S>
	using multisampled_layered_depth_stencil_buffer_view = basic_depth_stencil_buffer_view<S, true>;

}
