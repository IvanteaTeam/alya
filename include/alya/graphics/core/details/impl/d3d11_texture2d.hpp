#pragma once
#include<alya/graphics/core/details/impl/d3d11_texture2d_array.hpp>

namespace alya::graphics::core::details
{

	class d3d11_context;

	class d3d11_texture2d : public d3d11_texture2d_array
	{
	public:

		/*
		
		Creates empry texture.
		
		*/
		d3d11_texture2d(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t samples,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			d3d11_device& device
		) : d3d11_texture2d_array(
			width,
			height,
			mipmaps,
			1,
			samples,
			pixel,
			memory,
			bind,
			device
		) {}

		/*
		
		Creates 2d texture with spicified mipmaps.
		
		*/
		d3d11_texture2d(
			size_t width,
			size_t height,
			size_t mipmaps,
			const mipmap_source*init,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			d3d11_device&device
		) : d3d11_texture2d_array(
			width,
			height,
			mipmaps,
			1,
			init,
			pixel,
			memory,
			bind,
			device
		) {}

	};

}
