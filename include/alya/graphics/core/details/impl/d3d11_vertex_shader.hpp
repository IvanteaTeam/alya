#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<vector>

struct ID3D11Device;
struct ID3D11VertexShader;

namespace alya::graphics::core::details
{

	class d3d11_device;

	class d3d11_vertex_shader
	{
	public:

		using native_handle_type = windows::com::shared_ptr<ID3D11VertexShader>;
		using bytecode_type = std::vector<char>;

		d3d11_vertex_shader(const void*bytecode, size_t size, d3d11_device&);
		d3d11_vertex_shader(const d3d11_vertex_shader&) = delete;
		d3d11_vertex_shader(d3d11_vertex_shader&&) = default;
		d3d11_vertex_shader& operator=(const d3d11_vertex_shader&) = delete;
		d3d11_vertex_shader& operator=(d3d11_vertex_shader&&) = default;

		native_handle_type native_handle()const noexcept
		{
			return impl_;
		}

		const bytecode_type& bytecode()const noexcept
		{
			return bytecode_;
		}

	private:
		windows::com::shared_ptr<ID3D11VertexShader> impl_;
		std::vector<char> bytecode_;
	};

}
