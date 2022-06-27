#pragma once
#include<alya/async/promise.hpp>

namespace alya::resource
{

	template<typename T, typename Reader, typename Ex, typename...Args>
	concept async_loader_with = requires (std::string path, Reader reader, Ex ex, Args&&...args)
	{
		{T::async_load(path, reader, ex, std::forward<Args>(args)...)}->std::same_as<async::promise<typename T::resource_type>>;
	};

	template<typename T, typename Data, typename...Args>
	concept loader_with = requires(Data data, Args&&...args)
	{
		{T::load(data, std::forward<Args>(args)...) }->std::same_as<typename T::source_type>;
	};

}
