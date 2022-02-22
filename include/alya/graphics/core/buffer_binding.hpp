#pragma once
#include<d3d11.h>

namespace alya::graphics::core
{

	enum class buffer_binding
	{
		vertex_buffer = D3D11_BIND_VERTEX_BUFFER,
		index_buffer = D3D11_BIND_INDEX_BUFFER,
		constant_buffer = D3D11_BIND_CONSTANT_BUFFER
	};

}

namespace alya
{
	template<>
	struct is_bitmask_enum<graphics::core::buffer_binding> : std::true_type {};
}
