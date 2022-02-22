#include<alya/graphics/core/details/impl/d3d11_depth_stencil_view.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_depth_stencil_view::d3d11_depth_stencil_view(windows::com::shared_ptr<ID3D11Texture2D>texture)
	{
		windows::com::shared_ptr<ID3D11Device> device;
		texture->GetDevice(&device);

		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

		auto res = device->CreateDepthStencilView(texture.get(), &desc, &impl_);

		if (res != S_OK)
			throw std::system_error{ windows::make_error_code(res) };
	}

	d3d11_depth_stencil_view::d3d11_depth_stencil_view(windows::com::shared_ptr<ID3D11Texture2D>texture, size_t mipmap)
	{
		windows::com::shared_ptr<ID3D11Device> device;
		texture->GetDevice(&device);

		D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = mipmap;

		auto res = device->CreateDepthStencilView(texture.get(), &desc, &impl_);

		if (res != S_OK)
			throw std::system_error{ windows::make_error_code(res) };
	}

}
