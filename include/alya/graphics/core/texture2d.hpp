#pragma once
#include<alya/graphics/core/basic_texture2d.hpp>

namespace alya::graphics::core
{
	template<typename T>
	using texture2d = basic_texture2d<T, memory_qualifier::default_, bitmask{ texture_binding::color_buffer } | texture_binding::shader_resource, 1>;

	template<typename T, size_t S>
	using texture2dms = basic_texture2d<T, memory_qualifier::default_, bitmask{ texture_binding::color_buffer } | texture_binding::shader_resource, S>;

}
