#pragma once
#include<alya/graphics/core/details/impl/d3d11_device.hpp>
#include<alya/graphics/core/details/attribute_signature.hpp>

struct ID3D11InputLayout;

namespace alya::graphics::core::details
{

	class d3d11_input_layout
	{
	public:

		d3d11_input_layout(std::initializer_list<attribute_signature>, const void*vertex_shader_bytecode, size_t vertex_shader_bytecode_size, d3d11_device&);
		d3d11_input_layout(const d3d11_input_layout&) = delete;
		d3d11_input_layout(d3d11_input_layout&&) = default;
		d3d11_input_layout& operator=(const d3d11_input_layout&) = delete;
		d3d11_input_layout& operator=(d3d11_input_layout&&) = default;

		windows::com::shared_ptr<ID3D11InputLayout> native_handle()const noexcept
		{
			return impl_;
		}

	private:
		windows::com::shared_ptr<ID3D11InputLayout> impl_;
	};

}
