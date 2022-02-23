#pragma once
#include<alya/graphics/core/vertex_stream_base.hpp>
#include<alya/graphics/core/context_base.hpp>

namespace alya::graphics::core
{

	vertex_stream_base::vertex_stream_base(std::initializer_list<details::attribute_signature>signatures, const vertex_shader& shader, context_base& context)
		: input_layout_(signatures, shader.impl_.bytecode().data(), shader.impl_.bytecode().size(), context.device)
	{}

}
