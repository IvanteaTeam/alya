#pragma once
#include<boost/asio/execution/execute.hpp>
#include<alya/utility/function.hpp>

namespace alya::async::details
{
	
	template<typename Ex, typename F>
	void execute_impl(Ex&&ex, F&&f)
	{
		boost::asio::execution::execute(std::forward<Ex>(ex), alya::function<void()>{std::forward<F>(f)});
	}

}
