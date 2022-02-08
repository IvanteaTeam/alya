#pragma once 
#include<memory>
#include<mutex>
#include<alya/async/details/promise_handler_base.hpp>

namespace alya::async::details
{
	
	class promise_impl_base
	{
	public:

		inline promise_impl_base() : cancelled(false) {}
		virtual ~promise_impl_base() = default;

		inline bool is_cancelled()const noexcept
		{
			std::lock_guard g(mutex);
			return cancelled;
		}
		inline void cancel()noexcept
		{
			std::lock_guard g(mutex);
			cancelled = true;
			if (auto p = prev_.lock())
			{
				p->cancel();
			}
		}
		inline void set_prev(std::shared_ptr<promise_impl_base>p)noexcept
		{
			std::lock_guard g(mutex);
			if (cancelled)
			{
				p->cancel();
			}
			else
			{
				prev_ = p;
			}
		}

		bool cancelled;
		std::weak_ptr<promise_impl_base> prev_;
		mutable std::mutex mutex;
	};

	template<typename T>
	class promise_impl : public promise_impl_base
	{
	public:

		using value_type = T;

		promise_impl() noexcept : storage_(std::nullopt) {}

		~promise_impl()
		{
			if (!is_cancelled() && storage_.index() > 0 && handler_)
				handler_->handle(storage_);
		}

		template<typename U>
		void set_value(U&& v)
		{
			std::lock_guard g(mutex);
			storage_.emplace<promise_value<value_type>>(std::forward<U>(v));
		}

		template<typename U>
		void set_handler(U&& h)noexcept
		{
			std::lock_guard g(mutex);
			handler_ = std::forward<U>(h);
		}

		void set_exception(std::exception_ptr e)noexcept
		{
			std::lock_guard g(mutex);
			storage_.emplace<std::exception_ptr>(e);
		}

		bool is_ready()const noexcept
		{
			std::lock_guard g(mutex);
			return storage_.index() > 0;
		}

		promise_storage<value_type> storage_;
		std::unique_ptr<promise_handler_base<value_type>> handler_;
	};

}
