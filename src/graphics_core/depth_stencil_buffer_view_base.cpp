#include<alya/graphics/core/depth_stencil_buffer_view_base.hpp>
#include<alya/utility/windows/win32_error.hpp>

namespace alya::graphics::core
{

	depth_stencil_buffer_view_base::depth_stencil_buffer_view_base(texture2d_base& t)
		: ctx(t.get_device_context())
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		auto res = t.get_device()->CreateDepthStencilView(t.texture.get(), &desc, &dsv);
		if (res != S_OK)
			throw std::system_error{windows::make_error_code(res)};
	}

	depth_stencil_buffer_view_base::depth_stencil_buffer_view_base(texture2d_base& t, size_t m)
		: ctx(t.get_device_context())
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = m;
		auto res = t.get_device()->CreateDepthStencilView(t.texture.get(), &desc, &dsv);
		if (res != S_OK)
			throw std::system_error{ windows::make_error_code(res) };
	}

	void depth_stencil_buffer_view_base::clear_depth(float d)
	{
		ctx->ClearDepthStencilView(dsv.get(), D3D11_CLEAR_DEPTH, d, 0);
	}

}
