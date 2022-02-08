#pragma once
#include<alya/audio/basic_engine.hpp>
#include<boost/asio/system_executor.hpp>

namespace alya::audio
{
		
	using engine = basic_engine<
		core::wave_format<float, 2>, 
		boost::asio::basic_system_executor<
		boost::asio::execution::blocking_t::never_t,
		boost::asio::execution::relationship_t::fork_t,
		std::allocator<void>
		>
	>;

}
