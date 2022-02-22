#pragma once
#include<alya/graphics/core/depth_stencil_value.hpp>

namespace alya::graphics::core::details
{

	enum class pixel_type
	{
		si32_4 = 1,
		ui32_4,
		f32_4,

		si32_3,
		ui32_3,
		f32_3,

		si32_2,
		ui32_2,
		f32_2,

		si32_1,
		ui32_1,
		f32_1,

		si16_4,
		ui16_4,
		sn16_4,
		un16_4,
		f16_4,

		si16_2,
		ui16_2,
		sn16_2,
		un16_2,
		f16_2 ,

		si16_1,
		ui16_1,
		sn16_1,
		un16_1,
		f16_1 ,

		si8_4,
		ui8_4,
		sn8_4,
		un8_4,

		si8_2,
		ui8_2,
		sn8_2,
		un8_2,

		si8_1,
		ui8_1,
		sn8_1,
		un8_1,

		d16 ,
		d32 ,
		d24s8,

		none = 0
	};

	template<typename T>
	pixel_type make_pixel_type()
	{
		using enum pixel_type;

		auto normalized = [](size_t size, size_t sig, size_t count) constexpr
		{
			pixel_type
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
			pixel_type
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
			pixel_type
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
			pixel_type _32[] = { f32_1, f32_2, f32_3, f32_4 };

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

			if (is_normalized_v<el>)
				return normalized(sizeof(el), is_signed_v<el>, traits::element_count);
			else if (std::is_floating_point_v<el>)
				return float_(traits::element_count, sizeof(el));
			else if (is_signed_v<el>)
				return signed_int(traits::element_count, sizeof(el));
			else
				return unsigned_int(traits::element_count, sizeof(el));
		}
	}

}
