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

		using value_type = T;
		static constexpr memory_qualifier memory_qualifier = Q;
		static constexpr bitmask<texture_binding> binding = B;
		static constexpr size_t samples = S;

		
		basic_texture2d(
			size_t width, 
			size_t height,
			size_t mipmaps, 
			context_base& context
		) : texture2d_base(
			width,
			height,
			mipmaps,
			details::make_pixel_type<value_type>(),
			memory_qualifier,
			binding,
			context
		)
		{}

		
		basic_texture2d(
			std::forward_iterator auto mips_begin, 
			std::forward_iterator auto mips_end,
			context_base&context
		) : texture2d_base()
		{
			std::array<details::mipmap_source, 20> init{};
			
			size_t width, height, count;

			make_mipmaps_init(init.data(), mips_begin, mips_end, width, height, count);

			static_cast<texture2d_base&>(*this) = texture2d_base(
				width, 
				height,
				count, 
				memory_qualifier,
				binding,
				context
			);

		}
		
		basic_texture2d(generate_mipmaps_t, basic_image2d_view<value_type>image, context_base&context) : 
			texture2d_base(
				image.width(), 
				image.height(),
				details::make_pixel_type<value_type>(),
				memory_qualifier,
				binding,
				context
			)
		{
			write_mipmap(0, { image.data(), image.row_alignment(), 0 });
			generate_mipmaps();
		}
		
	private:

		static void make_mipmaps_init(details::mipmap_source*, std::forward_iterator auto, std::forward_iterator auto, size_t& width, size_t& height, size_t& count);

	};

	template<typename Pixel, memory_qualifier Q, texture_binding B, size_t S>
	void basic_texture2d<Pixel, Q, B, S>::make_mipmaps_init(details::mipmap_source*init, std::forward_iterator auto mips_begin, std::forward_iterator auto mips_end, size_t&width, size_t&height, size_t&count)
	{
		
		size_t w = 0, h = 0;

		auto next = [](auto n) { return n == 1 ? 1 : n / 2; };
		auto curr = init;
		for (auto it = mips_begin; it != mips_end; it++, curr++)
		{
			basic_image2d_view<value_type> image = *it;
			if (!w * !h)
			{
				w = image->width();
				h = image->height();
			}
			if (w == image->width() && h == image->height())
				*curr = { image.data(), image.row_alignment(), 0 };
			else
				throw bad_mipmaps{};
			w = next(w);
			h = next(h);
		}
		count = curr - init;
		width = mips_begin->width();
		height = mips_begin->height();
	}

}
