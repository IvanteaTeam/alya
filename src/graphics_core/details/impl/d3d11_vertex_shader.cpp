#include<alya/graphics/core/details/impl/d3d11_vertex_shader.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_vertex_shader::d3d11_vertex_shader(const void* data, size_t size, windows::com::shared_ptr<ID3D11Device>device)
		: bytecode_(static_cast<const char*>(data), static_cast<const char*>(data) + size)
	{
		ALYA_GFX_CALL(device->CreateVertexShader(data, size, nullptr, &impl_));
	
	}

}
