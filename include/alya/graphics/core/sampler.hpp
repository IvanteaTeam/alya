#pragma once
#include<alya/graphics/core/details/impl/d3d11_sampler.hpp>

namespace alya::graphics::core
{
	class context_base;

	class sampler
	{
	public:
	
		explicit sampler(context_base&);

	private:
		details::d3d11_sampler impl_;
		friend class context_base;
	};

}
