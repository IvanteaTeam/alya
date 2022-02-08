#pragma once
#include<alya/async/details/promise_impl.hpp>
#include<alya/async/details/promise_awaitable_handler.hpp>

namespace alya::async::details
{
	
	template<typename T>
	class awaitable
	{
	public:

		awaitable(std::shared_ptr<promise_impl<T>>impl) : impl_(impl), storage_(std::nullopt) {}
		
		bool await_ready()const noexcept
		{
			return impl_->is_ready();
		}

		template<typename U>
		void await_suspend(std::coroutine_handle<U>coro) noexcept
		{
			auto next = coro.promise().impl_;
			impl_->set_handler(std::make_unique<promise_awaitable_handler<T>>(storage_, coro));
			next->set_prev(impl_);
			impl_ = nullptr;
		}

		T await_resume()
		{
			if (impl_)
				storage_ = std::move(impl_->storage_);
			if (storage_.index() == 1)
			{
				if constexpr (std::is_void_v<T>)
					return;
				else
					return std::move(std::get<1>(storage_));
			}
			else
				std::rethrow_exception(std::get<2>(storage_));
		}

	private:
		std::shared_ptr<promise_impl<T>> impl_;
		promise_storage<T> storage_;
	};

}
