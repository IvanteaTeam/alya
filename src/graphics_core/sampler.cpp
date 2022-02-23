#include<alya/graphics/core/sampler.hpp>
#include<alya/graphics/core/context_base.hpp>

namespace alya::graphics::core
{

	sampler::sampler(context_base&context)
		: impl_(context.device)
	{}

}
