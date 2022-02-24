#include<alya/graphics/core/frame_buffer_base.hpp>
#include<alya/graphics/core/details/debug.hpp>

namespace alya::graphics::core
{
	/*
	void frame_buffer_base::clear_color(float r, float g, float b, float a, size_t index)
	{
		auto view = render_target_views_.at(index);
		if (view)
		{
			windows::com::shared_ptr<ID3D11Device> device;
			view->GetDevice(&device);
			windows::com::shared_ptr<ID3D11DeviceContext> context;
			device->GetImmediateContext(&context);
			float color[] = { r, g, b, a };
			context->ClearRenderTargetView(view.get(), color);
		}
	}

	void frame_buffer_base::clear_depth(float v)noexcept
	{
		if (depth_stencil_view_)
		{
			windows::com::shared_ptr<ID3D11Device> device;
			depth_stencil_view_->GetDevice(&device);
			windows::com::shared_ptr<ID3D11DeviceContext> context;
			device->GetImmediateContext(&context);
			context->ClearDepthStencilView(depth_stencil_view_.get(), D3D11_CLEAR_DEPTH, v, 0);
		}
	}
	*/
}
