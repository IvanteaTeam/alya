#pragma once
#include<alya/graphics/core/details/impl/d3d11_input_layout.hpp>
#include<alya/graphics/core/details/impl/d3d11_vertex_shader.hpp>

namespace alya::graphics::core::details
{

	class d3d11_vertex_stream_base
	{
	public:

		d3d11_vertex_stream_base(std::initializer_list<details::attribute_signature>signatures, const d3d11_vertex_shader& shader, windows::com::shared_ptr<ID3D11Device>);

	private:
		d3d11_input_layout input_layout_;
		friend class d3d11_context;
	};

}
