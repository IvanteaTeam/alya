#pragma once
#include<alya/graphics/core/details/pixel_type.hpp>

namespace alya::graphics::core::details
{

	struct attribute_signature
	{
		pixel_type type;
		const char* name;
		size_t index;
	};

}
