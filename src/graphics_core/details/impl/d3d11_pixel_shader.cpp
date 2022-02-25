#include<alya/graphics/core/details/impl/d3d11_pixel_shader.hpp>
#include<alya/graphics/core/details/impl/d3d11_context.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_pixel_shader::d3d11_pixel_shader(const void*bytecode, size_t size, d3d11_context&context)
	{
		ALYA_GFX_CALL(context.device().native_handle()->CreatePixelShader(bytecode, size, nullptr, &impl_));
	}

}
