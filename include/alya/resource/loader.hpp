#pragma once
#include<alya/async/promise.hpp>

namespace alya::resource
{
	
	template<typename T>
	concept loader = requires(T a)
	{
		typename T::data_type;
		typename T::source_type;
		{a.async_load(typename T::source_type{})}->std::same_as<async::promise<typename T::data_type>>;
	};

}
