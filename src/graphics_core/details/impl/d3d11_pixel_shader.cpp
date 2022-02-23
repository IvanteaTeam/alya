#include<alya/graphics/core/details/impl/d3d11_pixel_shader.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_pixel_shader::d3d11_pixel_shader(const void*data, size_t size, windows::com::shared_ptr<ID3D11Device>device)
	{
		ALYA_GFX_CALL(device->CreatePixelShader(data, size, nullptr, &impl_));
	}

}
