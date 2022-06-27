#pragma once
#include<concepts>
#include<string>
#include<vector>
#include<alya/async/promise.hpp>

namespace alya::resource
{

	template<typename T>
	concept reader = requires(T r, std::string path)
	{
		std::same_as<typename T::data_type, std::vector<char, typename T::allocator_type>>;
		{r.read(path) }-> std::same_as<typename T::data_type>;
	};

	template<typename T>
	concept async_reader = requires(T r, std::string path)
	{
		std::same_as<typename T::data_type, std::vector<char, typename T::allocator_type>>;
		{r.async_read(path)}->std::same_as<async::promise<typename T::data_type>>;
	};

}
