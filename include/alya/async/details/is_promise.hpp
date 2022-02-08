#pragma once
#include<type_traits>

namespace alya::async 
{

	template<typename T>
	class promise;

	template<typename T>
	struct is_promise : std::false_type {};

	template<typename T>
	struct is_promise<promise<T>> : std::true_type {};

	template<typename T>
	constexpr bool is_promise_v = is_promise<T>::value;

}
