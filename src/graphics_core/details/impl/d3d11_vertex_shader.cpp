#include<alya/graphics/core/details/impl/d3d11_vertex_shader.hpp>
#include<alya/graphics/core/details/impl/d3d11_context.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_vertex_shader::d3d11_vertex_shader(const void* bytecode, size_t size, d3d11_device&device)
		: bytecode_(static_cast<const char*>(bytecode), static_cast<const char*>(bytecode) + size)
	{
		ALYA_GFX_CALL(device.native_handle()->CreateVertexShader(bytecode, size, nullptr, &impl_));
	
	}

}
