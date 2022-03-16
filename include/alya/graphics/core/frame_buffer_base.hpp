#pragma once
#include<alya/graphics/core/texture2d_base.hpp>
#include<alya/graphics/core/color_buffer_view_base.hpp>
#include<alya/graphics/core/depth_stencil_buffer_view_base.hpp>
#include<alya/graphics/core/details/impl/d3d11_frame_buffer.hpp>

namespace alya::graphics::core
{

	class frame_buffer_base
	{
	public:

		explicit frame_buffer_base(context_base&context) : impl_(context.impl_) {};
		
		void attach_color_buffer(color_buffer_view_base*color_buffer, size_t slot)
		{
			//render_target_views_.at(slot) = color_buffer->impl_.native_handle();
			impl_.attach_color_buffer(color_buffer ? color_buffer->impl_.native_handle() : nullptr, slot);
		}

		void attach_depth_stencil_buffer(depth_stencil_buffer_view_base* depth_stencil_buffer)noexcept
		{
			//depth_stencil_view_ = depth_stencil_buffer ? depth_stencil_buffer->impl_.native_handle() : nullptr;
			impl_.attach_depth_stencil_buffer(depth_stencil_buffer ? depth_stencil_buffer->impl_.native_handle() : nullptr);
		}

		void clear_color(float r, float g, float b, float a, size_t index)
		{
			impl_.clear_color(r, g, b, a, index);
		}
		void clear_depth(float d)noexcept
		{
			impl_.clear_depth(d);
		}

	private:

		//std::array<windows::com::shared_ptr<ID3D11RenderTargetView>, 8> render_target_views_;
		//windows::com::shared_ptr<ID3D11DepthStencilView> depth_stencil_view_;

		details::d3d11_frame_buffer impl_;

		template<typename, typename, size_t>
		friend class basic_context;
	};

}
