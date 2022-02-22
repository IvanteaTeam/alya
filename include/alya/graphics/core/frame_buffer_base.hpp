#pragma once
#include<alya/graphics/core/texture2d_base.hpp>
#include<alya/graphics/core/color_buffer_view_base.hpp>
#include<alya/graphics/core/depth_stencil_buffer_view_base.hpp>

namespace alya::graphics::core
{

	class frame_buffer_base
	{
	public:

		explicit frame_buffer_base(context_base&) noexcept {};
		
		void attach_color_buffer(color_buffer_view_base*color_buffer, size_t slot)
		{
			render_target_views_.at(slot) = color_buffer->impl_.native_handle();
		}

		void attach_depth_stencil_buffer(depth_stencil_buffer_view_base* depth_stencil_buffer)noexcept
		{
			depth_stencil_view_ = depth_stencil_buffer ? depth_stencil_buffer->impl_.native_handle() : nullptr;
		}

		void clear_color(float r, float g, float b, float a, size_t index);
		void clear_depth(float)noexcept;

	private:

		std::array<windows::com::shared_ptr<ID3D11RenderTargetView>, 8> render_target_views_;
		windows::com::shared_ptr<ID3D11DepthStencilView> depth_stencil_view_;

		friend class context_base;
	};

}
