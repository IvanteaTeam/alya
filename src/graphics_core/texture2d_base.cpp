#include<alya/graphics/core/texture2d_base.hpp>
#include<alya/graphics/core/context_base.hpp>

namespace alya::graphics::core
{

	texture2d_base::texture2d_base(
		size_t width,
		size_t height,
		size_t mipmaps,
		size_t samples,
		details::pixel_type pixel,
		memory_qualifier memory,
		texture_binding bind,
		context_base& context
	) : 
		impl_(
			width,
			height,
			mipmaps,
			1,
			samples,
			pixel,
			memory,
			bind,
			context.device
		)
	{}

	texture2d_base::texture2d_base(
		size_t width,
		size_t height,
		details::pixel_type pixel,
		memory_qualifier memory,
		texture_binding bind,
		context_base& context
	)	:
		impl_(
			width, 
			height, 
			0,
			1, 
			nullptr,
			pixel, 
			memory,
			bind, 
			context.device
		)
	{}

}

