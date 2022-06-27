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

namespace alya::resource
{

	template<typename Resource, typename Reader, typename Executor, typename...Args>
	async::promise<Resource> async_load(std::string path, const Reader&reader, const Executor&work_ex, Args&&...args) requires async_reader<Reader>
	{
		using loader = typename loadable_traits<Resource>::loader;
		co_return co_await loader::async_load(path, reader, work_ex, std::forward<Args>(args)...);
	}

}
