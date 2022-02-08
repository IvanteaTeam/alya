#pragma once
#include<alya/graphics/core/memory_qualifier.hpp>
#include<alya/utility/bitmask.hpp>
#include<utility>
#include<d3d11.h>

namespace alya::graphics::core::d3d11
{

	enum class usage_flag
	{
		default_ = D3D11_USAGE::D3D11_USAGE_DEFAULT,
		dynamic = D3D11_USAGE::D3D11_USAGE_DYNAMIC,
		immutable = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
		staging = D3D11_USAGE::D3D11_USAGE_STAGING
	};

	enum class cpu_access_flag
	{
		none = 0,
		read = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ,
		write = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE,
	};

}

namespace alya
{

	template<>
	struct is_bitmask_enum<graphics::core::d3d11::cpu_access_flag> : std::true_type {};

}


namespace alya::graphics::core::d3d11
{

	constexpr std::pair<usage_flag, cpu_access_flag> convert_usage(memory_qualifier q)
	{
		using enum memory_qualifier;
		switch (q)
		{
		case default_:
			return { usage_flag::default_, cpu_access_flag::none };
		case dynamic:
			return { usage_flag::dynamic, cpu_access_flag::write };
		case staging:
			return { usage_flag::staging, bitmask{cpu_access_flag::read} | cpu_access_flag::write };
		case immutable:
			return { usage_flag::immutable, cpu_access_flag::none };
		default:
			return {};
		}
	}

}
