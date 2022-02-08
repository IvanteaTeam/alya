#pragma once
#include<alya/async/details/awaitable.hpp>

namespace alya::async::details
{
	
	template<typename T>
	class coroutine_promise
	{
	public:

		using value_type = T;

		coroutine_promise() : impl_(std::make_shared<promise_impl<value_type>>()) {}

		std::shared_ptr<promise_impl<T>> get_return_object()
		{
			return impl_;
		}

		std::suspend_never initial_suspend()noexcept { return {}; }
		std::suspend_never final_suspend()noexcept { return {}; }

		template<typename U>
		void return_value(U&&v)
		{
			impl_->set_value(std::forward<U>(v));
		}

		void unhandled_exception() noexcept
		{
			impl_->set_exception(std::current_exception());
		}

		std::shared_ptr<promise_impl<value_type>> impl_;
	};

	template<>
	class coroutine_promise<void>
	{
	public:

		using value_type = void;

		coroutine_promise() : impl_(std::make_shared<promise_impl<value_type>>()) {}

		std::shared_ptr<promise_impl<value_type>> get_return_object()
		{
			return impl_;
		}

		std::suspend_never initial_suspend()noexcept { return {}; }
		std::suspend_never final_suspend()noexcept { return {}; }

		void return_void()noexcept
		{
			impl_->set_value(std::monostate{});
		}

		void unhandled_exception() noexcept
		{
			impl_->set_exception(std::current_exception());
		}

		std::shared_ptr<promise_impl<value_type>> impl_;
	};

}
