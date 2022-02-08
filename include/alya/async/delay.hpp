#pragma once 
#include<alya/async/promise.hpp>
#include<chrono>

namespace alya::async
{
	
	promise<std::chrono::milliseconds> delay_for(std::chrono::milliseconds);

}
