#pragma once 
#include<alya/graphics/core/memory_qualifier.hpp>
#include<d3d11.h>
#include<utility>

namespace alya::graphics::core::details
{

	constexpr std::pair<D3D11_USAGE, D3D11_CPU_ACCESS_FLAG> d3d11_usage(memory_qualifier memory)
	{
		using enum memory_qualifier;
		switch (memory)
		{
		case default_:
			return { D3D11_USAGE_DEFAULT, D3D11_CPU_ACCESS_FLAG(0) };
		case dynamic:
			return { D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE };
		case staging:
			return { D3D11_USAGE_STAGING, D3D11_CPU_ACCESS_FLAG(D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE) };
		case immutable:
			return { D3D11_USAGE_IMMUTABLE, D3D11_CPU_ACCESS_FLAG(0) };
		default:
			return {};
		}
	}

}
