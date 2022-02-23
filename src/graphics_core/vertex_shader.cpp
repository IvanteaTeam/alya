#include<alya/graphics/core/vertex_shader.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<alya/resource/bad_format.hpp>
#include<alya/graphics/core/context_base.hpp>

namespace alya::graphics::core
{
	vertex_shader::vertex_shader(const void*data, size_t size, context_base&context)
		: impl_(data, size, context.device)
	{}

}
