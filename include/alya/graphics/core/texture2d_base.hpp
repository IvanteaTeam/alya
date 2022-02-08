#pragma once
#include<alya/graphics/core/details/d3d11/object_base.hpp>
#include<alya/graphics/core/details/dxgi/format.hpp>
#include<alya/utility/image_view.hpp>
#include<alya/graphics/core/memory_qualifier.hpp>
#include<alya/graphics/core/exceptions.hpp>
#include<vector>
#include<alya/graphics/core/details/d3d11/d3d.hpp>
#include<alya/utility/bitmask.hpp>

namespace alya::graphics::core
{

	class bad_mipmaps : public ::std::exception {};

	enum class texture_binding
	{
		color_buffer = D3D11_BIND_RENDER_TARGET,
		shader_resource = D3D11_BIND_SHADER_RESOURCE,
		depth_stencil_buffer = D3D11_BIND_DEPTH_STENCIL
	};

	struct generate_mipmaps_t {};

	constexpr generate_mipmaps_t generate_mipmaps{};

	class texture2d_base : protected d3d11::object_base
	{
	public:
		

		template<typename C, typename T>
		texture2d_base(
			size_t width,
			size_t height,
			size_t levels,
			size_t slices,
			size_t samples,
			T*,
			memory_qualifier Q,
			texture_binding B,
			C&&ctx
		) : texture2d_base(
			{ {}, slices, levels},
			width,
			height,
			samples,
			false,
			dxgi::convert_format<T>(),
			Q, 
			B,
			get_device(ctx),
			get_device_context(ctx)
		) 
		{}

		template<typename C, typename T>
		texture2d_base(
			std::forward_iterator auto begin, 
			std::forward_iterator auto end, 
			T*,
			memory_qualifier Q, 
			texture_binding B, 
			C&&ctx
		) : texture2d_base(
			make_init(begin, end, (T*)nullptr),
			begin->width(),
			begin->height(),
			1,
			false,
			dxgi::convert_format<T>(),
			Q,
			B,
			get_device(ctx),
			get_device_context(ctx)
		)
		{}

		template<typename C, typename T>
		texture2d_base(
			generate_mipmaps_t,
			std::forward_iterator auto begin,
			std::forward_iterator auto end,
			T*,
			memory_qualifier Q,
			texture_binding B,
			C&& ctx
		) : texture2d_base(
			make_init(begin, end, (T*)nullptr),
			begin->width(),
			begin->height(),
			1,
			true,
			dxgi::convert_format<T>(),
			Q,
			B,
			get_device(ctx),
			get_device_context(ctx)
		)
		{}
		
		size_t mipmaps()const noexcept;
		size_t width(size_t mipmap = 0)const noexcept;
		size_t height(size_t mipmap = 0)const noexcept;

	protected:
		size_t layers()const noexcept;

	private:
		using mip_t = std::pair<const void*, size_t>;
		using init_t = std::tuple<std::vector<mip_t>, size_t, size_t>;


		template<typename T>
		static init_t make_init(std::forward_iterator auto b, std::forward_iterator auto e, T*)
		{
			using image_t = basic_image2d_view<T>;
			
			std::vector<mip_t> out;

			size_t w0 = b->width(), h0 = b->height();
			size_t levels = 0;

			for (auto it = b; it != e; it++, levels++)
				if (it != b && it->width() >= (it - 1)->width())
					break;

			for (auto it = b; it != e;)
			{
				image_t img = *it;
				size_t w = img.width();
				size_t h = img.height();
				if (!(w == w0 && h == h0))
					throw std::invalid_argument{ "" };
				
				for (size_t i = 0; i < levels && it != e; i++, it++)
				{
					if (!(it->width() == w && it->height() == h))
						throw bad_mipmaps{};
					out.push_back({ it->data(), it->row_alignment() });
					w /= 2;
					h /= 2;
					
				};
				
			}
			return { out, out.size() / levels, 0};
		}

		texture2d_base(
			const init_t& init,
			size_t width,
			size_t height,
			size_t samples,
			bool generate_mipmaps,
			dxgi::format,
			memory_qualifier,
			texture_binding,
			d3d11::device_ptr,
			d3d11::device_context_ptr
		);

		size_t width_, height_, mipmaps_, layers_;
		mutable d3d11::texture2d_ptr texture;

		friend class context_base;
		friend class texture_view;
		friend class color_buffer_view_base;
		friend class depth_stencil_buffer_view_base;
	};
	
}


namespace alya
{
	template<>
	struct is_bitmask_enum<alya::graphics::core::texture_binding> : std::true_type {};
}
