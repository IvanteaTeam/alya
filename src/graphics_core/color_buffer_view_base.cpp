#include<alya/graphics/core/color_buffer_view_base.hpp>
#include<alya/utility/windows/win32_error.hpp>

namespace alya::graphics::core
{

	color_buffer_view_base::color_buffer_view_base(texture2d_base& t)
		: ctx(t.get_device_context())
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		auto res = t.get_device()->CreateRenderTargetView(t.texture.get(), &desc, &rtv);
		if (res != S_OK)
			throw std::system_error{ windows::make_error_code(res) };
	}

	color_buffer_view_base::color_buffer_view_base(texture2d_base& t, size_t m)
		: ctx(t.get_device_context())
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = m;
		auto res = t.get_device()->CreateRenderTargetView(t.texture.get(), &desc, &rtv);
		if (res != S_OK)
			throw std::system_error{ windows::make_error_code(res) };
	}

	void color_buffer_view_base::clear_color(float r, float g, float b, float a)
	{
		float color[] = { r, g, b, a };
		ctx->ClearRenderTargetView(rtv.get(), color);
	}

}
