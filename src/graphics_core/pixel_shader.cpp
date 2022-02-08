#include<alya/graphics/core/pixel_shader.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<alya/resource/bad_format.hpp>

namespace alya
{
	namespace graphics::core
	{
		pixel_shader::pixel_shader(const void* data, size_t size, d3d11::device_ptr device, d3d11::device_context_ptr ctx)
			: object_base(device, ctx)
		{
			auto res = ALYA_GFX_CALL((get_device()->CreatePixelShader(data, size, nullptr, &shader)));
			if (res != S_OK)
				throw resource::bad_format{};
		}
	}
}
