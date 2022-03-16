#pragma once
#include<alya/graphics/core/details/impl/d3d11_input_layout.hpp>
#include<alya/graphics/core/details/impl/d3d11_vertex_shader.hpp>

struct ID3D11Buffer;

namespace alya::graphics::core::details
{

	template<size_t Size>
	class d3d11_vertex_stream
	{
	public:

		d3d11_vertex_stream(const std::array<attribute_signature, Size>&signatures, const d3d11_vertex_shader&shader, d3d11_device&device)
			: d3d11_vertex_stream(std::make_index_sequence<Size>{}, signatures, shader, device)
		{}

		void attach_attribute_stream(windows::com::shared_ptr<ID3D11Buffer>buffer, size_t offset, size_t stride, size_t index)
		{
			buffers_.at(index) = buffer;
			offsets_.at(index) = offset;
			strides_.at(index) = stride;
		}

	private:

		template<size_t...I>
		d3d11_vertex_stream(std::index_sequence<I...>, const std::array<attribute_signature, Size>& signatures, const d3d11_vertex_shader& shader, d3d11_device&device)
			: input_layout_({ signatures[I]... }, shader.bytecode().data(), shader.bytecode().size(), device), buffers_{}, offsets_{}, strides_{}
		{}

		d3d11_input_layout input_layout_;
		std::array<windows::com::shared_ptr<ID3D11Buffer>, Size> buffers_;
		std::array<uint32_t, Size> offsets_, strides_;
		friend class d3d11_context;
	};

}
