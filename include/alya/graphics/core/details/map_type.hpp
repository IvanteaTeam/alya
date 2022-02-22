#pragma once
#include<alya/utility/bitmask.hpp>

namespace alya::graphics::core::details
{

	enum class map_type
	{
		write,
		read,
		read_write,
		write_discard
	};

}
