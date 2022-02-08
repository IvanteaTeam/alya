#pragma once
#include<alya/async/details/promise_handler_base.hpp>
#include<coroutine>

namespace alya::async::details
{

	template<typename T>
	class promise_awaitable_handler : public promise_handler_base<T>
	{
	public:

		promise_awaitable_handler(promise_storage<T>&dest, std::coroutine_handle<>coro) : dest_(dest), called(false), coro_(coro) {}

		~promise_awaitable_handler()
		{
			if (!called)
				coro_.destroy();
		}

		virtual void handle(promise_storage<T>&storage)noexcept
		{
			called = true;
			dest_ = std::move(storage);
			coro_.resume();
		}

	private:
		std::coroutine_handle<> coro_;
		promise_storage<T>& dest_;
		bool called;
	};

}
