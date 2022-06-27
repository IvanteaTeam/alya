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
		{r.read(path) }-> std::same_as<std::vector<char>>;
	};

	template<typename T>
	concept async_reader = requires(T r, std::string path)
	{
		{r.async_read(path)}->std::same_as<async::promise<std::vector<char>>>;
	};

}
