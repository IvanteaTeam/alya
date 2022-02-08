#pragma once
#include<alya/async/details/is_promise.hpp>

namespace alya::async::details
{

	template<typename T>
	constexpr bool is_promise_value_v = std::is_void_v<T> || std::is_nothrow_move_constructible_v<T> && std::is_nothrow_destructible_v<T>;

	template<typename F, typename T>
	struct promise_handler_invoke_result
	{
		static constexpr auto impl()
		{
			if constexpr (std::is_void_v<T>)
				return std::declval<std::invoke_result_t<F>>();
			else
				return std::declval<std::invoke_result_t<F, T>>();
		}
		using type = decltype(impl());
	};

	template<typename F, typename T>
	using promise_handler_invoke_result_t = typename promise_handler_invoke_result<F, T>::type;

	template<typename F1, typename F2, typename T>
	struct promise_next_value_type
	{
		static constexpr auto impl()
		{
			using A = promise_handler_invoke_result_t<F1, T>;
			using B = promise_handler_invoke_result_t<F2, std::exception_ptr>;

			if constexpr (is_promise_v<A> && is_promise_v<B>)
			{
				static_assert(std::is_same_v<typename A::value_type, typename B::value_type>);
				return std::declval<typename A::value_type>();
			}
			else if constexpr (is_promise_v<A> && !is_promise_v<B>)
			{
				static_assert(std::is_same_v<typename A::value_type, B>);
				return std::declval<typename A::value_type>();
			}
			else if constexpr (!is_promise_v<A> && is_promise_v<B>)
			{
				static_assert(std::is_same_v<A, typename B::value_type>);
				return std::declval<A>();
			}
			else if constexpr (!is_promise_v<A> && !is_promise_v<B>)
			{
				static_assert(std::is_same_v<A, B>);
				return std::declval<A>();
			}

		}
		using type = decltype(impl());
	};

	template<typename F1, typename F2, typename T>
	using promise_next_value_type_t = typename promise_next_value_type<F1, F2, T>::type;

}
