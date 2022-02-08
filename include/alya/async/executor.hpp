#pragma once
#include<boost/asio/execution/executor.hpp>

namespace alya::async
{
	
	template<typename T>
	concept executor = boost::asio::execution::executor<T>;

}
