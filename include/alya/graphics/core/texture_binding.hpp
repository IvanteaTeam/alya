#pragma once
#include<alya/utility/bitmask.hpp>
#include<d3d11.h>

namespace alya::graphics::core
{

	enum class texture_binding
	{
		color_buffer = D3D11_BIND_RENDER_TARGET,
		shader_resource = D3D11_BIND_SHADER_RESOURCE,
		depth_stencil_buffer = D3D11_BIND_DEPTH_STENCIL
	};

}

namespace alya
{
	template<>
	struct is_bitmask_enum<alya::graphics::core::texture_binding> : std::true_type {};
}
