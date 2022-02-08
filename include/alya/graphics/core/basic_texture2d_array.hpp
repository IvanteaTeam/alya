#pragma once
#include<alya/graphics/core/memory_qualifier.hpp>
#include<alya/graphics/core/texture2d_base.hpp>

namespace alya::graphics::core
{

	template<typename T, memory_qualifier Q, texture_binding B, size_t S>
	class basic_texture2d_array : public texture2d_base
	{
	public:
		/*
		template<typename C>
		basic_texture2d_array(size_t w, size_t h, size_t l, C&& ctx) : texture2d_base(w, h, l, 1, S, details::make_dxgi_format<pixel_traits<T>>(), Q, std::forward<C>(c)) {}

		template<typename C>
		basic_texture2d_array(size_t w, size_t h, C&& ctx) : basic_texture2d_array(w, h, 1, std::forward<C>(ctx)) {}

		template<typename C>
		basic_texture2d_array(std::forward_iterator auto mips_begin, std::forward_iterator auto mips_end, size_t levels, C&& c);

		*/

	private:

	};

}
