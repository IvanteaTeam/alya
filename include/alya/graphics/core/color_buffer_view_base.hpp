#pragma once
#include<alya/graphics/core/texture2d_base.hpp>
#include<alya/graphics/core/details/impl/d3d11_render_target_view.hpp>

namespace alya::graphics::core
{
	class color_buffer_view_base
	{
	public:

		color_buffer_view_base(texture2d_base&texture, size_t mipmap) : 
			impl_(texture.impl_.native_handle(), mipmap)
		{}

		color_buffer_view_base(texture2d_base&texture) :
			impl_(texture.impl_.native_handle())
		{}

	private:
		details::d3d11_render_target_view impl_;
		friend class frame_buffer_base;
	};
}
