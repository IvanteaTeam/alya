#pragma once
#include<alya/async/promise.hpp>
#include<alya/resource/importable.hpp>
#include<alya/resource/loader.hpp>

namespace alya::resource
{

	template<importable I, loader L, async::executor Ex, typename...Args>
	async::promise<I> async_import(L&loader, typename L::source_type source, Ex&&ex, Args&&...args)
	{
		using Importer = typename importable_traits<I>::importer;
		return Importer()(
			loader, 
			std::move(source),
			std::forward<Ex>(ex), 
			std::forward<Args>(args)...
			);
	}

}
