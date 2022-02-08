#pragma once

namespace alya::async::details
{

	template<typename F, typename T>
	class promise_callback_wrapper
	{
	public:
		promise_callback_wrapper(F&&f) noexcept : f(std::move(f)) {}
		promise_callback_wrapper(const F& f) : f(f) {}

		template<typename U>
		constexpr auto operator()(U&& v)
		{
			if constexpr (std::is_void_v<decltype(f(std::forward<U>(v)))>)
			{
				f(std::forward<U>(v));
				return std::monostate{};
			}
			else
			{
				return f(std::forward<U>(v));
			}
		}

	private:
		F f;
	};

	template<typename F>
	class promise_callback_wrapper<F, void>
	{
	public:
		promise_callback_wrapper(F&& f) noexcept : f(std::move(f)) {}
		//promise_callback_wrapper(const F& f) : f(f) {}

		constexpr auto operator()(std::monostate)
		{
			if constexpr (std::is_void_v<decltype(f())>)
			{
				f();
				return std::monostate{};
			}
			else
			{
				return f();
			}
		}

	private:
		F f;
	};

}
