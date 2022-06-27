/*#pragma once
#include<alya/resource/loader.hpp>

namespace alya::resource
{
	
	template<loader Loader>
	async::promise<typename Loader::data_type> async_load(Loader&loader, const typename Loader::source_type&source)
	{
		return loader.async_load(source);
	}

}
*/

#pragma once
#include<alya/async/promise.hpp>
#include<alya/resource/loadable_traits.hpp>
#include<alya/resource/reader.hpp>
#include<alya/resource/loader.hpp>
#include<alya/async/execute.hpp>

namespace alya::resource
{

	template<typename Resource, typename Reader, typename Executor, typename...Args>
	async::promise<Resource> async_load(std::string path, Reader reader, const Executor&work_ex, Args&&...args) requires async_reader<Reader>
	{
		using loader = typename loadable_traits<Resource>::loader;
		if constexpr (async_loader_with<loader, Reader, Executor, Args...>)
			co_return co_await loader::async_load(path, reader, work_ex, std::forward<Args>(args)...);
		else
		{
			auto data = co_await reader.async_read(path);
			co_return co_await async::execute(work_ex, [&]() {
				return loader::load(data, std::forward<Args>(args)...);
			});
		}
	}

}
