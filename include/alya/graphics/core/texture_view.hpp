#pragma once
#include<alya/graphics/core/basic_texture2d.hpp>

namespace alya::graphics::core
{

	class texture_view
	{
	public:

		template<typename T, memory_qualifier Q, texture_binding B, size_t S>
		texture_view(basic_texture2d<T, Q, B, S>&texture)requires(bitmask(B).contains(texture_binding::shader_resource) && S > 1)
			: texture_view(static_cast<texture2d_base&>(texture))
		{}

		template<typename T, memory_qualifier Q, texture_binding B>
		texture_view(basic_texture2d<T, Q, B, 1>& texture, size_t first, size_t count)requires(bitmask(B).contains(texture_binding::shader_resource))
			: texture_view(static_cast<texture2d_base&>(texture), first, count)
		{}

	private:

		texture_view(texture2d_base&, size_t mips_begin, size_t mips_count);
		texture_view(texture2d_base&);

		mutable d3d11::srv_ptr srv;
		friend class context_base;
	};

}
