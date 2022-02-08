#pragma once
#include<d3d11.h>
#include<alya/utility/pixel_traits.hpp>
#include<alya/graphics/depth_stencil_value.hpp>

namespace alya::graphics::dxgi
{

	enum class format
	{
		// 4 bytes [ si, f, ui ]
		// 2 bytes [si, ui, sn, un, f]
		// 1 byte [si, ui, sn, un, un_srgb]

		si32_4 = DXGI_FORMAT_R32G32B32A32_SINT,
		ui32_4 = DXGI_FORMAT_R32G32B32A32_UINT,
		f32_4 = DXGI_FORMAT_R32G32B32A32_FLOAT,

		si32_3 = DXGI_FORMAT_R32G32B32_SINT,
		ui32_3 = DXGI_FORMAT_R32G32B32_UINT,
		f32_3 = DXGI_FORMAT_R32G32B32_FLOAT,

		si32_2 = DXGI_FORMAT_R32G32_SINT,
		ui32_2 = DXGI_FORMAT_R32G32_UINT,
		f32_2 = DXGI_FORMAT_R32G32_FLOAT,

		si32_1 = DXGI_FORMAT_R32_SINT,
		ui32_1 = DXGI_FORMAT_R32_UINT,
		f32_1 = DXGI_FORMAT_R32_FLOAT,

		si16_4 = DXGI_FORMAT_R16G16B16A16_SINT,
		ui16_4 = DXGI_FORMAT_R16G16B16A16_UINT,
		sn16_4 = DXGI_FORMAT_R16G16B16A16_SNORM,
		un16_4 = DXGI_FORMAT_R16G16B16A16_UNORM,
		f16_4 = DXGI_FORMAT_R16G16B16A16_FLOAT,

		si16_2 = DXGI_FORMAT_R16G16_SINT,
		ui16_2 = DXGI_FORMAT_R16G16_UINT,
		sn16_2 = DXGI_FORMAT_R16G16_SNORM,
		un16_2 = DXGI_FORMAT_R16G16_UNORM,
		f16_2 = DXGI_FORMAT_R16G16_FLOAT,

		si16_1 = DXGI_FORMAT_R16_SINT,
		ui16_1 = DXGI_FORMAT_R16_UINT,
		sn16_1 = DXGI_FORMAT_R16_SNORM,
		un16_1 = DXGI_FORMAT_R16_UNORM,
		f16_1 = DXGI_FORMAT_R16_FLOAT,

		si8_4 = DXGI_FORMAT_R8G8B8A8_SINT,
		ui8_4 = DXGI_FORMAT_R8G8B8A8_UINT,
		sn8_4 = DXGI_FORMAT_R8G8B8A8_SNORM,
		un8_4 = DXGI_FORMAT_R8G8B8A8_UNORM,

		si8_2 = DXGI_FORMAT_R8G8_SINT,
		ui8_2 = DXGI_FORMAT_R8G8_UINT,
		sn8_2 = DXGI_FORMAT_R8G8_SNORM,
		un8_2 = DXGI_FORMAT_R8G8_UNORM,

		si8_1 = DXGI_FORMAT_R8_SINT,
		ui8_1 = DXGI_FORMAT_R8_UINT,
		sn8_1 = DXGI_FORMAT_R8_SNORM,
		un8_1 = DXGI_FORMAT_R8_UNORM,

		d16 = DXGI_FORMAT_D16_UNORM,
		d32 = DXGI_FORMAT_D32_FLOAT,
		d24s8 = DXGI_FORMAT_D24_UNORM_S8_UINT,

		none = 0
	};

	template<typename T>
	constexpr format convert_format()
	{
		using enum format;

		auto normalized = [](size_t size, size_t sig, size_t count) constexpr
		{
			format
				signed_8[] = { sn8_1, sn8_2, none, sn8_4 },
				unsigned_8[] = { un8_1, un8_2, none, un8_4 },
				signed_16[] = { sn16_1, sn16_2, none, sn16_4 },
				unsigned_16[] = { un16_1, un16_2, none, un16_4 };

			if (size == 1 && sig)
				return signed_8[count - 1];
			else if (size == 1 && !sig)
				return unsigned_8[count - 1];
			else if (size == 2 && sig)
				return signed_16[count - 1];
			else if (size == 2 && !sig)
				return unsigned_16[count - 1];
			else
				return none;
		};

		auto signed_int = [](size_t count, size_t size)constexpr
		{
			format
				_8[] = { si8_1, si8_2, none, si8_4 },
				_16[] = { si16_1, si16_2, none, si16_4 },
				_32[] = { si32_1, si32_2, si32_3, si32_4 };

			if (size == 1)
				return _8[count - 1];
			else if (size == 2)
				return _16[count - 1];
			else if (size == 4)
				return _32[count - 1];
			else
				return none;
		};

		auto unsigned_int = [](size_t count, size_t size)constexpr
		{
			format
				_8[] = { ui8_1, ui8_2, none, ui8_4 },
				_16[] = { ui16_1, ui16_2, none, ui16_4 },
				_32[] = { ui32_1, ui32_2, ui32_3, ui32_4 };

			if (size == 1)
				return _8[count - 1];
			else if (size == 2)
				return _16[count - 1];
			else if (size == 4)
				return _32[count - 1];
			else
				return none;
		};

		auto float_ = [](size_t count, size_t size)constexpr
		{
			format _32[] = { f32_1, f32_2, f32_3, f32_4 };

			if (size == 4)
				return _32[count - 1];
			else
				return none;
		};

		auto depth_stencil = [](size_t d, size_t s) {
			if (d == 32 && s == 0)
				return d32;
			if (d == 24 && s == 8)
				return d24s8;
			if (d == 16 && s == 8)
				return d16;
			return none;
		};

		using traits = pixel_traits<T>;

		if constexpr (is_depth_stencil_value<typename traits::value_type>::value)
		{
			return depth_stencil(traits::depth_bits, traits::stencil_bits);
		}
		else
		{
			using el = typename traits::element_type;

			if (is_normalized<el>::value)
				return normalized(sizeof(el), is_signed<el>::value, traits::element_count);
			else if (std::is_floating_point_v<el>)
				return float_(traits::element_count, sizeof(el));
			else if (is_signed<el>::value)
				return signed_int(traits::element_count, sizeof(el));
			else
				return unsigned_int(traits::element_count, sizeof(el));
		}
	}

}
