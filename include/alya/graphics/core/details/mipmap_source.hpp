#pragma once
#include<array>

namespace alya::graphics::core::details
{

	struct mipmap_source
	{
		const void* data;
		size_t row_pitch;
		size_t slice_pitch;
	};

}
