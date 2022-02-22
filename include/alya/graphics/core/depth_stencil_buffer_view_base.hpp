#pragma once
#include<alya/graphics/core/texture2d_base.hpp>
#include<alya/graphics/core/details/impl/d3d11_depth_stencil_view.hpp>

namespace alya::graphics::core
{
	
	class depth_stencil_buffer_view_base
	{
	public:
		
		depth_stencil_buffer_view_base(texture2d_base&texture) : 
			impl_(texture.impl_.native_handle())
		{}
		depth_stencil_buffer_view_base(texture2d_base& texture, size_t mipmap) : 
			impl_(texture.impl_.native_handle(), mipmap)
		{}

	private:
		details::d3d11_depth_stencil_view impl_;
		friend class frame_buffer_base;
	};

}
