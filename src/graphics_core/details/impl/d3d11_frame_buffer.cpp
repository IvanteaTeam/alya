#include<alya/graphics/core/details/impl/d3d11_frame_buffer.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	void d3d11_frame_buffer::do_clear_color(windows::com::shared_ptr<ID3D11RenderTargetView>view, const float*color)noexcept
	{
		windows::com::shared_ptr<ID3D11Device> device;
		view->GetDevice(&device);
		windows::com::shared_ptr<ID3D11DeviceContext> context;
		device->GetImmediateContext(&context);
		context->ClearRenderTargetView(view.get(), color);
	}

	void d3d11_frame_buffer::do_clear_depth(windows::com::shared_ptr<ID3D11DepthStencilView>view, float depth)noexcept
	{
		windows::com::shared_ptr<ID3D11Device> device;
		view->GetDevice(&device);
		windows::com::shared_ptr<ID3D11DeviceContext> context;
		device->GetImmediateContext(&context);
		context->ClearDepthStencilView(view.get(), D3D11_CLEAR_DEPTH, depth, 0);
	}

}
