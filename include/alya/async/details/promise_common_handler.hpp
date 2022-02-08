#pragma once
#include<alya/async/details/promise_propagate_state_handler.hpp>
#include<alya/async/details/promise_next_value_type.hpp>
#include<alya/async/details/promise_callback_wrapper.hpp>
#include<alya/async/details/execute_impl.hpp>
#include<functional>

namespace alya::async::details
{
	
	template<typename F1, typename F2, typename T, typename Ex>
	class promise_common_handler : public promise_handler_base<T>
	{
	public:

		using R = promise_next_value_type_t<F1, F2, T>;
		using A = promise_callback_wrapper<F1, T>;
		using B = promise_callback_wrapper<F2, std::exception_ptr>;

		template<typename A, typename B, typename Ex>
		promise_common_handler(A&& f1, B&& f2, Ex&&ex, std::shared_ptr<promise_impl<R>>n)
			: a_(std::forward<A>(f1)), b_(std::forward<B>(f2)), ex_(std::forward<Ex>(ex)), next_(n)
		{}

		virtual void handle(promise_storage<T>&storage)noexcept override
		{
			try {
				if (storage.index() == 1)
					execute_callback(std::move(a_), std::move(std::get<1>(storage)));
				else
					execute_callback(std::move(b_), std::move(std::get<2>(storage)));
			}
			catch (...)
			{
				next_->set_exception(std::current_exception());
			}
		}

	private:

		template<typename F, typename T>
		static void invoke_callback(F f, T v, std::shared_ptr<promise_impl<R>> next)noexcept
		{
			try {
				using U = decltype(f(std::move(v)));
				if constexpr (is_promise_v<U>)
				{
					auto i = f(std::move(v)).impl_;
					i->set_handler(std::make_unique<promise_propagate_state_handler<typename U::value_type>>(next));
					next->set_prev(i);
				}
				else
				{
					next->set_value(f(std::move(v)));
				}
			}
			catch (...)
			{
				next->set_exception(std::current_exception());
			}
		}

		template<typename F, typename T>
		void execute_callback(F&&f, T&&v)
		{
			execute_impl(ex_, [f = std::forward<F>(f), v = std::forward<T>(v), next = next_]()mutable {
				invoke_callback<F, T>(std::move(f), std::move(v), next);
			});
			//execute_impl(ex_, std::bind(invoke_callback<F, T>, std::forward<F>(f), std::forward<T>(v), next_));
		}

		A a_;
		B b_;
		Ex ex_;
		std::shared_ptr<promise_impl<R>> next_;
	};


}
