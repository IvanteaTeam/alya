#pragma once
#include<alya/async/details/promise_storage.hpp>

namespace alya::async::details
{
	
	template<typename T>
	class promise_handler_base
	{
	public:

		using value_type = T;

		virtual void handle(promise_storage<value_type>&) noexcept = 0;

		virtual ~promise_handler_base() = default;
	};

}
