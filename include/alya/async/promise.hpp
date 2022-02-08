#pragma once
#include<mutex>
#include<variant>
#include<coroutine>
#include<memory>
#include<alya/async/details/promise_storage.hpp>
#include<alya/async/details/promise_next_value_type.hpp>
#include<alya/async/details/promise_callback_wrapper.hpp>
#include<alya/async/details/promise_propagate_state_handler.hpp>
#include<alya/async/details/promise_common_handler.hpp>
#include<alya/async/inline_executor.hpp>
#include<alya/async/executor.hpp>
#include<alya/async/details/coroutine_promise.hpp>

namespace alya::async
{

	template<typename>
	class defer;

	template<typename T>
	class promise
	{
	public:

		using value_type = T;
		
		template<typename F, executor Ex = inline_executor>
		constexpr auto then(F&& f, Ex&& ex = Ex{})
		{
			return then(
				std::forward<F>(f), 
				[](auto e)->details::promise_handler_invoke_result_t<F, value_type> { std::rethrow_exception(e); },
				std::forward<Ex>(ex)
			);
		}

		template<typename F, executor Ex = inline_executor>
		constexpr auto except(F f, Ex&& ex = Ex{})
		{
			return then(
				[]<typename...T>(T&&...val) { if constexpr (sizeof...(T) == 0)return; else return (std::forward<T>(val), ...); },
				std::forward<F>(f),
				std::forward<Ex>(ex)
			);
		}

		template<typename F1, std::invocable<std::exception_ptr> F2, executor Ex = inline_executor>
		constexpr auto then(F1&& f1, F2&& f2, Ex&& ex = Ex{})
		{
			using U = details::promise_next_value_type_t<F1, F2, value_type>;
			return promise<U>(then_impl<U>(
				std::forward<F1>(f1),
				std::forward<F2>(f2),
				std::forward<Ex>(ex)
			));
		}

		void cancel()noexcept
		{
			impl_->cancel();
			impl_ = nullptr;
		}

		bool is_ready()const noexcept
		{
			return impl_->is_ready();
		}

		promise(promise&&) = default;
		promise(const promise&) = delete;
		promise& operator=(promise&&) = default;
		promise& operator=(const promise&) = default;

		details::awaitable<value_type> operator co_await()
		{
			return std::move(impl_);
		}

		class promise_type : public details::coroutine_promise<value_type>
		{
		public:
			promise get_return_object()
			{
				return details::coroutine_promise<value_type>::get_return_object();
			}
		};

	private:

		template<typename T, typename F1, typename F2, typename Ex>
		constexpr auto then_impl(F1&&f1, F2&&f2, Ex&&ex)
		{
			auto next = std::make_shared<details::promise_impl<T>>();
			impl_->set_handler(std::make_unique<details::promise_common_handler<F1, F2, value_type, Ex>>(std::forward<F1>(f1), std::forward<F2>(f2), std::forward<Ex>(ex), next));
			next->set_prev(impl_);

			return next;
		}

		promise(std::shared_ptr<details::promise_impl<value_type>>i) : impl_(i) {} 

		std::shared_ptr<details::promise_impl<value_type>> impl_;
		template<typename, typename, typename, typename>
		friend class details::promise_common_handler;
		template<typename>
		friend constexpr auto make_promise();
		template<typename>
		friend class promise;
	};

	template<typename T>
	class defer
	{
	public:
		template<typename U>
		void set_value(U&& v)
		{
			impl_->set_value(std::forward<U>(v));
			impl_ = nullptr;
		}
		void set_exception(std::exception_ptr e)noexcept
		{
			impl_->set_exception(e);
			impl_ = nullptr;
		}
		template<typename E>
		void set_exception(E&& e)noexcept
		{
			try
			{
				throw std::forward<E>(e);
			}
			catch (...)
			{
				set_exception(std::current_exception());
			}
		}
		bool is_cancelled()const noexcept
		{
			return impl_->is_cancelled();
		}
	private:
		defer(std::shared_ptr<details::promise_impl<T>> i) : impl_(i) {}
		std::shared_ptr<details::promise_impl<T>> impl_;
		template<typename T>
		friend constexpr auto make_promise();
	};

	template<>
	class defer<void>
	{
	public:
		void set_value()
		{
			impl_->set_value(std::monostate{});
			impl_ = nullptr;
		}
		void set_exception(std::exception_ptr e)noexcept
		{
			impl_->set_exception(e);
			impl_ = nullptr;
		}
		template<typename E>
		void set_exception(E&& e)noexcept
		{
			try
			{
				throw std::forward<E>(e);
			}
			catch (...)
			{
				set_exception(std::current_exception());
			}
		}
		bool is_cancelled()const noexcept
		{
			return impl_->is_cancelled();
		}
	private:
		defer(std::shared_ptr<details::promise_impl<void>> i) : impl_(i) {}
		std::shared_ptr<details::promise_impl<void>> impl_;
		template<typename T>
		friend constexpr auto make_promise();
	};


	template<typename T>
	constexpr auto make_promise()
	{
		auto i = std::make_shared<details::promise_impl<T>>();
		return std::pair{ promise<T>(i), defer<T>(i) };
	}

}
