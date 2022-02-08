#pragma once
#include<alya/graphics/core/memory_qualifier.hpp>
#include<alya/graphics/core/texture2d_base.hpp>

namespace alya::graphics::core
{

	template<typename T, memory_qualifier Q, texture_binding B, size_t S>
	class basic_texture2d;

	template<typename T, memory_qualifier Q, texture_binding B, size_t S>
	class basic_texture2d : public texture2d_base
	{
	public:

		template<typename C>
		basic_texture2d(
			size_t width, 
			size_t height,
			size_t levels, 
			C&& ctx
		) : texture2d_base(
			width,
			height, 
			levels,
			1,
			S,
			(T*)nullptr,
			Q,
			B,
			std::forward<C>(ctx)
		) 
		{}

		template<typename C>
		basic_texture2d(size_t width, size_t height, C&& ctx) : basic_texture2d(width, height, 1, std::forward<C>(ctx)) {}

		template<typename C>
		basic_texture2d(
			std::forward_iterator auto mips_begin, 
			std::forward_iterator auto mips_end,
			C&&ctx
		) : texture2d_base(
			mips_begin,
			mips_end,
			(T*)nullptr,
			Q,
			B,
			std::forward<C>(ctx)
		)
		{}

		template<typename C>
		basic_texture2d(basic_image2d_view<T>image, C&& ctx) : basic_texture2d(&image, &image + 1, std::forward<C>(ctx)) {}

		template<typename C>
		basic_texture2d(generate_mipmaps_t, basic_image2d_view<T>image, C&&ctx) : texture2d_base(
			generate_mipmaps,
			&image,
			&image + 1,
			(T*)nullptr,
			Q,
			B,
			std::forward<C>(ctx)
		)
		{}
	private:

	};

}
