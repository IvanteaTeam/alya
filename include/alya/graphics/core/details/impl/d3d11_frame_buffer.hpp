#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<array>

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace alya::graphics::core::details
{

	class d3d11_frame_buffer
	{
	public:

		d3d11_frame_buffer()noexcept = default;
		d3d11_frame_buffer(const d3d11_frame_buffer&) = delete;
		d3d11_frame_buffer(d3d11_frame_buffer&&) = default;
		d3d11_frame_buffer& operator=(const d3d11_frame_buffer&) = delete;
		d3d11_frame_buffer& operator=(d3d11_frame_buffer&&) = default;

		void attach_color_buffer(windows::com::shared_ptr<ID3D11RenderTargetView>view, size_t index)
		{
			render_buffers_.at(index) = view;
		}

		void attach_depth_stencil_buffer(windows::com::shared_ptr<ID3D11DepthStencilView>view)noexcept
		{
			depth_stencil_buffer_ = view;
		}

		void clear_color(float r, float g, float b, float a, size_t index)
		{
			auto view = render_buffers_.at(index);
			float color[] = { r, g, b, a };
			if(view)
				do_clear_color(view, color);
		}

		void clear_depth(float d)noexcept
		{
			if (depth_stencil_buffer_)
				do_clear_depth(depth_stencil_buffer_, d);
		}

	private:
		
		static void do_clear_color(windows::com::shared_ptr<ID3D11RenderTargetView>, const float*)noexcept;
		static void do_clear_depth(windows::com::shared_ptr<ID3D11DepthStencilView>, float)noexcept;

		std::array<windows::com::shared_ptr<ID3D11RenderTargetView>, 8> render_buffers_ = {};
		windows::com::shared_ptr<ID3D11DepthStencilView> depth_stencil_buffer_;
		friend class d3d11_context;
	};

}
