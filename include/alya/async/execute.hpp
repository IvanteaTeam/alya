#pragma once
#include<alya/async/promise.hpp>
#include<alya/utility/function.hpp>
#include<concepts>

namespace alya::async
{
	
	template<typename Ex, std::invocable<> F>
	constexpr auto execute(Ex&& ex, F&&f)
	{
		
		using T = decltype(std::invoke(f));

		auto [p, d] = make_promise<T>();

		//details::execute_impl(std::forward<Ex>(ex), []() {});
		
		details::execute_impl(std::forward<Ex>(ex), [f = std::forward<F>(f), d = std::move(d)]()mutable {

			try
			{
				if constexpr (std::is_void_v<T>)
				{
					f();
					d.set_value();
				}
				else
				{
					d.set_value(
						f()
					);
				}
			}
			catch (...)
			{

			}

			});
			

		return std::move(p);
	}

}
