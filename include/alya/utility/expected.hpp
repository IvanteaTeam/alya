#pragma once
#include<tl/expected.hpp>

namespace alya
{
	using namespace tl;

	template<typename T>
	struct is_expected : std::false_type {};

	template<typename T, typename E>
	struct is_expected<alya::expected<T, E>> : std::true_type {};

	template<typename T>
	constexpr bool is_expected_v = is_expected<T>::value;

}
