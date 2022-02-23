#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>

struct ID3D11Device;
struct ID3D11PixelShader;

namespace alya::graphics::core::details
{

	class d3d11_pixel_shader
	{
	public:

		using native_handle_type = windows::com::shared_ptr<ID3D11PixelShader>;

		d3d11_pixel_shader(const void* bytecode, size_t size, windows::com::shared_ptr<ID3D11Device>device);
		d3d11_pixel_shader(const d3d11_pixel_shader&) = delete;
		d3d11_pixel_shader(d3d11_pixel_shader&&) = default;
		d3d11_pixel_shader& operator=(const d3d11_pixel_shader&) = delete;
		d3d11_pixel_shader& operator=(d3d11_pixel_shader&&) = default;

		native_handle_type native_handle()const noexcept
		{
			return impl_;
		}

	private:
		windows::com::shared_ptr<ID3D11PixelShader> impl_;
	};

}
