#pragma once
#include<alya/graphics/core/details/impl/d3d11_vertex_stream_base.hpp>
#include<alya/graphics/core/context_base.hpp>

namespace alya::graphics::core::details
{

	d3d11_vertex_stream_base::d3d11_vertex_stream_base(std::initializer_list<details::attribute_signature>signatures, const d3d11_vertex_shader& shader, windows::com::shared_ptr<ID3D11Device>device)
		: input_layout_(signatures, shader.bytecode().data(), shader.bytecode().size(), device)
	{

	}

}
