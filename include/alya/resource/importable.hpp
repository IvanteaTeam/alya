#pragma once
#include<alya/resource/loader.hpp>

namespace alya::resource
{

	template<typename T>
	struct importable_traits
	{
		using importer = typename T::importer;
	};

	template<typename T>
	concept importable = requires
	{
		typename importable_traits<T>::importer;
	};

	template<typename Importable, typename Loader, typename...Args>
	concept importable_from = importable<Importable> && requires(typename importable_traits<Importable>::importer i, const typename Loader::data_type & data)
	{
		{i(data, std::declval<Args>()...)}->std::same_as<Importable>;
	};

	template<typename Importable, typename Loader, typename Executor, typename...Args>
	concept async_importable_from = importable<Importable> && 
		requires(
		typename importable_traits<Importable>::importer i, 
		Loader & loader, 
		const typename Loader::source_type & source,
		const Executor & executor
		)
	{
		{i(
			loader, 
			source, 
			executor,
			std::declval<Args>()...
		)}->std::same_as<async::promise<Importable>>;
	};

}
