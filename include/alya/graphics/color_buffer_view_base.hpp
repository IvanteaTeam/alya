#pragma once
#include<alya/graphics/texture2d_base.hpp>

namespace alya::graphics
{
	class color_buffer_view_base
	{
	public:

		color_buffer_view_base(texture2d_base&, size_t m);
		color_buffer_view_base(texture2d_base&);

		void clear_color(float, float, float, float);

	private:
		d3d11::rtv_ptr rtv;
		d3d11::device_context_ptr ctx;
		friend class frame_buffer_base;
	};
}
