#pragma once
#include<alya/graphics/texture2d_base.hpp>

namespace alya::graphics
{
	
	class depth_stencil_buffer_view_base
	{
	public:
		
		depth_stencil_buffer_view_base(texture2d_base&texture);
		depth_stencil_buffer_view_base(texture2d_base& texture, size_t mipmap);

		void clear_depth(float);

	private:
		d3d11::dsv_ptr dsv;
		d3d11::device_context_ptr ctx;
		friend class frame_buffer_base;
	};

}
