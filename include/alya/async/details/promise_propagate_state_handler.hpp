#pragma once
#include<alya/async/details/promise_impl.hpp>

namespace alya::async::details
{

	template<typename T>
	class promise_propagate_state_handler : public promise_handler_base<T>
	{
	public:
		using value_type = T;

		promise_propagate_state_handler(auto n) : next_(n) {}

		virtual void handle(promise_storage<value_type>& storage)noexcept override
		{
			if (storage.index() == 1)
				next_->set_value(std::move(std::get<1>(storage)));
			else
				next_->set_exception(std::get<2>(storage));
		}
		std::shared_ptr<promise_impl<T>> next_;
	};

}
