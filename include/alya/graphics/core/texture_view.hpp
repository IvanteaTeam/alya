#pragma once
#include<alya/graphics/core/basic_texture2d.hpp>
#include<alya/graphics/core/details/impl/d3d11_shader_resource_view.hpp>

namespace alya::graphics::core
{

	class texture_view
	{
	public:

		template<typename T, memory_qualifier Q, texture_binding B, size_t S>
		texture_view(basic_texture2d<T, Q, B, S>&texture)requires(bitmask(B).contains(texture_binding::shader_resource) && S > 1)  :
			impl_(texture.impl_.native_handle())
		{}

		template<typename T, memory_qualifier Q, texture_binding B>
		texture_view(basic_texture2d<T, Q, B, 1>& texture, size_t first, size_t count)requires(bitmask(B).contains(texture_binding::shader_resource)) :
			impl_(texture.impl_.native_handle(), first, count)
		{}

	private:

		details::d3d11_shader_resource_view impl_;
		friend class context_base;
	};

}
