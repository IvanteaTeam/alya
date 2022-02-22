#pragma once
#include<alya/graphics/core/details/pixel_type.hpp>
#include<dxgi.h>

namespace alya::graphics::core::details
{

	constexpr DXGI_FORMAT dxgi_format(pixel_type type)
	{
		using enum pixel_type;
		switch (type)
		{
		case si32_4: return DXGI_FORMAT_R32G32B32A32_SINT;
		case ui32_4: return DXGI_FORMAT_R32G32B32A32_UINT;
		case f32_4: return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case si32_3: return DXGI_FORMAT_R32G32B32_SINT;
		case ui32_3: return DXGI_FORMAT_R32G32B32_UINT;
		case f32_3: return  DXGI_FORMAT_R32G32B32_FLOAT;

		case si32_2: return  DXGI_FORMAT_R32G32_SINT;
		case ui32_2: return DXGI_FORMAT_R32G32_UINT;
		case f32_2: return DXGI_FORMAT_R32G32_FLOAT;

		case si32_1: return DXGI_FORMAT_R32_SINT;
		case ui32_1: return  DXGI_FORMAT_R32_UINT;
		case f32_1: return  DXGI_FORMAT_R32_FLOAT;

		case si16_4: return DXGI_FORMAT_R16G16B16A16_SINT;
		case ui16_4: return DXGI_FORMAT_R16G16B16A16_UINT;
		case sn16_4: return  DXGI_FORMAT_R16G16B16A16_SNORM;
		case un16_4: return  DXGI_FORMAT_R16G16B16A16_UNORM;
		case f16_4: return  DXGI_FORMAT_R16G16B16A16_FLOAT;

		case si16_2: return DXGI_FORMAT_R16G16_SINT;
		case ui16_2: return  DXGI_FORMAT_R16G16_UINT;
		case sn16_2: return  DXGI_FORMAT_R16G16_SNORM;
		case un16_2: return  DXGI_FORMAT_R16G16_UNORM;
		case f16_2: return  DXGI_FORMAT_R16G16_FLOAT;

		case si16_1: return  DXGI_FORMAT_R16_SINT;
		case ui16_1: return  DXGI_FORMAT_R16_UINT;
		case sn16_1: return  DXGI_FORMAT_R16_SNORM;
		case un16_1: return  DXGI_FORMAT_R16_UNORM;
		case f16_1: return DXGI_FORMAT_R16_FLOAT;

		case si8_4: return  DXGI_FORMAT_R8G8B8A8_SINT;
		case ui8_4: return DXGI_FORMAT_R8G8B8A8_UINT;
		case sn8_4: return  DXGI_FORMAT_R8G8B8A8_SNORM;
		case un8_4: return  DXGI_FORMAT_R8G8B8A8_UNORM;

		case si8_2: return  DXGI_FORMAT_R8G8_SINT;
		case ui8_2: return  DXGI_FORMAT_R8G8_UINT;
		case sn8_2: return  DXGI_FORMAT_R8G8_SNORM;
		case un8_2: return  DXGI_FORMAT_R8G8_UNORM;

		case si8_1: return  DXGI_FORMAT_R8_SINT;
		case ui8_1: return  DXGI_FORMAT_R8_UINT;
		case sn8_1: return  DXGI_FORMAT_R8_SNORM;
		case un8_1: return DXGI_FORMAT_R8_UNORM;

		case d16: return  DXGI_FORMAT_D16_UNORM;
		case d32: return  DXGI_FORMAT_D32_FLOAT;
		case d24s8: return  DXGI_FORMAT_D24_UNORM_S8_UINT;
		default:
			return DXGI_FORMAT(0);
		};
	}

}
