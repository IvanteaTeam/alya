#pragma once
#include<alya/graphics/core/details/impl/d3d11_device.hpp>
#include<alya/graphics/core/details/mipmap_source.hpp>
#include<alya/graphics/core/details/pixel_type.hpp>
#include<alya/graphics/core/memory_qualifier.hpp>
#include<alya/graphics/core/texture_binding.hpp>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace alya::graphics::core::details
{

	class d3d11_texture2d_array
	{
	public:

		using native_handle_type = windows::com::shared_ptr<ID3D11Texture2D>;

		/*
		
		Internal constructor.
		Creates empty 2d texture array or cube texture array
		
		*/
		d3d11_texture2d_array(
			size_t width,
			size_t height,
			size_t mipmaps, 
			size_t layers, 
			size_t samples,
			bool cube,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			windows::com::shared_ptr<ID3D11Device> device
		);

		/*
		
		Internal constructor.
		Creates 2d texture array or cube texture array with spicified mipmaps.
		
		*/
		d3d11_texture2d_array(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t layers,
			bool cube,
			const mipmap_source*init,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			windows::com::shared_ptr<ID3D11Device> device
		);
		
		/*
		
		Internal constructor.
		Creates empty 2d texture array or cube texture array.
		
		*/
		d3d11_texture2d_array(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t layers,
			size_t samples,
			bool cube,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			d3d11_device& device
		) : d3d11_texture2d_array(
			width,
			height,
			mipmaps,
			layers,
			samples,
			cube,
			pixel,
			memory,
			bind,
			device.native_handle()
		)
		{}

		/*
		
		Internal constructor.
		Creates 2d texture array or cube texture array with spicified mipmaps.
		
		*/
		d3d11_texture2d_array(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t layers,
			bool cube,
			const mipmap_source*init,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			d3d11_device& device
		) : d3d11_texture2d_array(
			width,
			height,
			mipmaps,
			layers,
			cube,
			init,
			pixel,
			memory,
			bind,
			device.native_handle()
		)
		{}


		/*
		
		Creates empty 2d texture array.
		
		*/
		d3d11_texture2d_array(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t layers,
			size_t samples,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			d3d11_device&device
		) : d3d11_texture2d_array(
			width,
			height,
			mipmaps,
			layers,
			samples,
			false,
			pixel,
			memory,
			bind,
			device
		)
		{}

		/*
		
		Creates 2d texture array with spicified mipmaps.
		
		*/
		d3d11_texture2d_array(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t layers,
			const mipmap_source*init,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			d3d11_device& device
		) : d3d11_texture2d_array(
			width,
			height,
			mipmaps,
			layers,
			false,
			init,
			pixel,
			memory,
			bind,
			device
		)
		{}

		d3d11_texture2d_array(const d3d11_texture2d_array&) = delete;
		d3d11_texture2d_array(d3d11_texture2d_array&&) = default;
		d3d11_texture2d_array& operator=(const d3d11_texture2d_array&) = delete;
		d3d11_texture2d_array& operator=(d3d11_texture2d_array&&) = default;

		void write_mipmap(mipmap_source, size_t layer, size_t mipmap);
		void generate_mipmaps();

		size_t width()const noexcept
		{
			return width_;
		}

		size_t height()const noexcept
		{
			return height_;
		}

		size_t mipmaps()const noexcept
		{
			return mipmaps_;
		}

		size_t layers()const noexcept
		{
			return layers_;
		}

		size_t samples()const noexcept
		{
			return samples_;
		}

		native_handle_type native_handle()const noexcept
		{
			return impl_;
		}

	private:
		windows::com::shared_ptr<ID3D11Texture2D> impl_;
		windows::com::shared_ptr<ID3D11ShaderResourceView> generate_mipmaps_view_;
		size_t width_, height_, mipmaps_, layers_, samples_;
	};

}
