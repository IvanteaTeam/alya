#pragma once
#include<alya/utility/normalized.hpp>

namespace alya::audio::core::details
{
	
	enum class sample_type
	{
		int16n_t,
		int32n_t,
		float_t
	};

	constexpr bool is_sample_floating_point(sample_type s)
	{
		using enum sample_type;
		return s == float_t;
	}

	constexpr size_t size_of_sample(sample_type s)
	{
		using enum sample_type;
		return s == int16n_t ? 2 : 4;
	}

	template<typename T>
	constexpr sample_type make_sample_type();

	template<>
	constexpr sample_type make_sample_type<int16n_t>()
	{
		return sample_type::int16n_t;
	}

	template<>
	constexpr sample_type make_sample_type<int32n_t>()
	{
		return sample_type::int32n_t;
	}

	template<>
	constexpr sample_type make_sample_type<float>()
	{
		return sample_type::float_t;
	}

}
