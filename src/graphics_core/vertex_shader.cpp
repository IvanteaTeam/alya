#include<alya/graphics/core/vertex_shader.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<alya/resource/bad_format.hpp>

namespace alya::graphics::core
{
	vertex_shader::vertex_shader(const void*data, size_t size, d3d11::device_ptr device, d3d11::device_context_ptr ctx)
		: object_base(device, ctx), bytecode(reinterpret_cast<const char*>(data), reinterpret_cast<const char*>(data) + size)
	{
		auto res = ALYA_GFX_CALL(get_device()->CreateVertexShader(data, size, nullptr, &shader));
		if (res != S_OK)
			throw resource::bad_format{};
	}
	
}
