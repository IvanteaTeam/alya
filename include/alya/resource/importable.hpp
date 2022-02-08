#pragma once

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
		typename importable_traits<T>;
	};

}
