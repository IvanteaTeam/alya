#pragma once
#include<alya/graphics/core/basic_texture2d.hpp>
#include<alya/graphics/core/color_buffer_view_base.hpp>

namespace alya::graphics::core
{

	template<size_t S, bool L>
	class basic_color_buffer_view;

	template<size_t S>
	class basic_color_buffer_view<S, false> : public color_buffer_view_base
	{
	public:
		template<typename T, memory_qualifier Q, texture_binding B>
		basic_color_buffer_view(basic_texture2d<T, Q, B, S>& texture) requires(bitmask(B).contains(texture_binding::color_buffer))
			: color_buffer_view_base(texture)
		{}
	};

	template<>
	class basic_color_buffer_view<1, false> : public color_buffer_view_base
	{
	public:
		template<typename T, memory_qualifier Q, texture_binding B>
		basic_color_buffer_view(basic_texture2d<T, Q, B, 1>& texture, size_t mipmap) requires(bitmask(B).contains(texture_binding::color_buffer))
			: color_buffer_view_base(texture, mipmap)
		{}
	};

}
