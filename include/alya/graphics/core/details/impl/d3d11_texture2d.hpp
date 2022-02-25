#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<alya/graphics/core/details/mipmap_source.hpp>
#include<alya/graphics/core/details/pixel_type.hpp>
#include<alya/graphics/core/memory_qualifier.hpp>
#include<alya/graphics/core/texture_binding.hpp>

struct ID3D11Texture2D;
struct ID3D11Device;
struct ID3D11ShaderResourceView;

namespace alya::graphics::core::details
{

	class d3d11_context;

	class d3d11_texture2d
	{
	public:

		d3d11_texture2d() = default;

		d3d11_texture2d(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t layers,
			size_t samples,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			windows::com::shared_ptr<ID3D11Device>
		);

		d3d11_texture2d(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t layers,
			size_t samples,
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			d3d11_context&
		);

		d3d11_texture2d(
			size_t width,
			size_t height,
			size_t mipmaps,
			size_t layers,
			const details::mipmap_source*init, 
			pixel_type pixel,
			memory_qualifier memory,
			texture_binding bind,
			d3d11_context&
		);

		d3d11_texture2d(const d3d11_texture2d&) = delete;
		d3d11_texture2d(d3d11_texture2d&&) = default;
		d3d11_texture2d& operator=(const d3d11_texture2d&) = delete;
		d3d11_texture2d& operator=(d3d11_texture2d&&) = default;

		void update(size_t mipmap, size_t layer, details::mipmap_source)noexcept;
		void generate_mipmaps()noexcept;

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

		windows::com::shared_ptr<ID3D11Texture2D> native_handle()const noexcept
		{
			return impl_;
		}

	private:
		windows::com::shared_ptr<ID3D11Texture2D> impl_;
		windows::com::shared_ptr<ID3D11ShaderResourceView> generate_mipmaps_view_;
		size_t width_, height_, mipmaps_, layers_;
	};

}
