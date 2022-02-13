#pragma once
#include<alya/resource/importable.hpp>
#include<alya/resource/loader.hpp>
#include<alya/async/execute.hpp>
#include<alya/resource/async_load.hpp>

namespace alya::resource
{

	template<
		importable Importable,
		loader Loader,
		async::executor Executor,
		typename...Args
	>
	async::promise<Importable> async_import(
		Loader&loader, 
		typename Loader::source_type source, 
		Executor&&executor, 
		Args&&...args
	)
	{
		using Importer = typename importable_traits<Importable>::importer;
		if constexpr (importable_from<Importable, Loader, Args&&...>)
		{
			auto data = co_await async_load(loader, source);
			co_return co_await async::execute(executor, [&]() {
				Importer i;
				return i(data, std::forward<Args>(args)...);
			});
		}
		else if constexpr (async_importable_from<Importable, Loader, Executor, Args&&...>)
		{
			Importer i;
			co_return co_await i(loader, source, executor, std::forward<Args>(args)...);
		}
	}

}
