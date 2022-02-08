#pragma once
#include<variant>
#include<optional>

namespace alya::async::details
{

	template<typename T>
	using promise_value = std::conditional_t<std::is_void_v<T>, std::monostate, T>;

	template<typename T>
	using promise_storage = std::variant<std::nullopt_t, promise_value<T>, std::exception_ptr>;

}
