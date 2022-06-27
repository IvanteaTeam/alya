#pragma once

namespace alya::resource
{

	template<typename T>
	struct loadable_traits
	{
		using loader = typename T::loader;
	};

}
