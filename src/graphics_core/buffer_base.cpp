#include<alya/graphics/core/buffer_base.hpp>
#include<alya/graphics/core/context_base.hpp>

namespace alya::graphics::core
{

	buffer_base::buffer_base(const void*init, size_t size, memory_qualifier memory, buffer_binding bind, context_base&context) : 
		impl_(init, size, memory, bind, context.device)
	{

	}

}
