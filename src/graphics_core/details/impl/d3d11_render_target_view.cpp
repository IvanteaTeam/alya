#include<alya/graphics/core/details/impl/d3d11_render_target_view.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_render_target_view::d3d11_render_target_view(windows::com::shared_ptr<ID3D11Texture2D>texture)
	{
		windows::com::shared_ptr<ID3D11Device> device;
		texture->GetDevice(&device);

		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		
		auto res = device->CreateRenderTargetView(texture.get(), &desc, &impl_);

		if (res != S_OK)
			throw std::system_error{ windows::make_error_code(res) };
	}
	d3d11_render_target_view::d3d11_render_target_view(windows::com::shared_ptr<ID3D11Texture2D>texture, size_t mipmap)
	{
		windows::com::shared_ptr<ID3D11Device> device;
		texture->GetDevice(&device);

		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = mipmap;

		auto res = device->CreateRenderTargetView(texture.get(), &desc, &impl_);

		if (res != S_OK)
			throw std::system_error{ windows::make_error_code(res) };
	}

}
