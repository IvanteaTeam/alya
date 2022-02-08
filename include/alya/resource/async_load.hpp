#pragma once
#include<alya/resource/loader.hpp>

namespace alya::resource
{
	
	template<loader L>
	async::promise<typename L::data_type> async_load(L&l, typename L::source_type s)
	{
		return l.async_load(std::move(s));
	}

}
