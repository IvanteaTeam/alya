#pragma once
#include<alya/graphics/core/details/pixel_type.hpp>
#include<alya/utility/image_view.hpp>
#include<alya/graphics/core/memory_qualifier.hpp>
#include<alya/graphics/core/exceptions.hpp>
#include<vector>
#include<alya/utility/bitmask.hpp>
#include<alya/graphics/core/details/impl/d3d11_texture2d.hpp>
#include<alya/graphics/core/context_base.hpp>

namespace alya::graphics::core
{

	class context_base;

	class bad_mipmaps : public std::logic_error 
	{
	public:
		bad_mipmaps() : logic_error("Invalid mipmaps.") {}
	};
	
	struct generate_mipmaps_t {};

	constexpr generate_mipmaps_t generate_mipmaps{};

	class texture2d_base
	{
	public:
		/*
		
		Creates texture with unspecified data
		
		*/
		texture2d_base(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t samples,
			details::pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			context_base&context
		) : impl_(width, height, mipmaps, samples, pixel, memory, bind, context.impl_)
		{}

		/*
		
		Creates texture with specified mipmaps
		
		*/
		texture2d_base(
			size_t width,
			size_t height,
			size_t mipmaps,
			const details::mipmap_source*init,
			details::pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			context_base&context
		) : impl_(width, height, mipmaps, init, pixel, memory, bind, context.impl_)
		{}

		/*
		
		Creates texture with unspecified count of mipmaps(for mipmaps generating purpose)
		
		*/
		texture2d_base(
			size_t width,
			size_t height,
			details::pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			context_base&context
		) : texture2d_base(width, height, 0, 1, pixel, memory, bind, context)
		{}

		size_t mipmaps()const noexcept
		{
			return impl_.mipmaps();
		}
		size_t width()const noexcept
		{
			return impl_.width();
		}
		size_t height()const noexcept
		{
			return impl_.height();
		}

	protected:

		void write_mipmap(size_t index, details::mipmap_source image)
		{
			impl_.write_mipmap(image, 0, index);
		}
		
		void generate_mipmaps()noexcept
		{
			impl_.generate_mipmaps();
		}

	private:
		
		details::d3d11_texture2d impl_;

		friend class context_base;
		friend class texture_view;
		friend class color_buffer_view_base;
		friend class depth_stencil_buffer_view_base;
	};
	
}

