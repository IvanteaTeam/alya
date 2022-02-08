#pragma once
#include<alya/graphics/core/depth_stencil_buffer_view_base.hpp>
#include<alya/graphics/core/basic_texture2d.hpp>

namespace alya::graphics::core
{

	template<size_t S, bool L>
	class basic_depth_stencil_buffer_view;

	template<size_t S>
	class basic_depth_stencil_buffer_view<S, false> : public depth_stencil_buffer_view_base
	{
	public:
		template<size_t D, size_t St, memory_qualifier Q, texture_binding B>
		basic_depth_stencil_buffer_view(basic_texture2d<depth_stencil_value<D, St>, Q, B, S>&texture) 
			: depth_stencil_buffer_view_base(texture)
		{}
	};

	template<>
	class basic_depth_stencil_buffer_view<1, false> : public depth_stencil_buffer_view_base
	{
	public:
		template<size_t D, size_t St, memory_qualifier Q, texture_binding B>
		basic_depth_stencil_buffer_view(basic_texture2d<depth_stencil_value<D, St>, Q, B, 1>& texture, size_t mipmap) 
			: depth_stencil_buffer_view_base(texture, mipmap)
		{}
	};

}
