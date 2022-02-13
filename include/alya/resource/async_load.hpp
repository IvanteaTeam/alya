#pragma once
#include<alya/resource/loader.hpp>

namespace alya::resource
{
	
	template<loader Loader>
	async::promise<typename Loader::data_type> async_load(Loader&loader, const typename Loader::source_type&source)
	{
		return loader.async_load(source);
	}

}
