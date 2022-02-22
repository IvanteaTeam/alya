#include<alya/graphics/core/details/impl/d3d11_shader_resource_view.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_shader_resource_view::d3d11_shader_resource_view(windows::com::shared_ptr<ID3D11Texture2D> texture)
	{

		windows::com::shared_ptr<ID3D11Device> device;

		texture->GetDevice(&device);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;

		auto res = device->CreateShaderResourceView(texture.get(), &desc, &impl_);
		if (res != S_OK)
			throw std::system_error{ windows::make_error_code(res) };
	}

	d3d11_shader_resource_view::d3d11_shader_resource_view(windows::com::shared_ptr<ID3D11Texture2D> texture, size_t first, size_t count)
	{
		windows::com::shared_ptr<ID3D11Device> device;
		texture->GetDevice(&device);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = first;
		desc.Texture2D.MipLevels = count;

		auto res = device->CreateShaderResourceView(texture.get(), &desc, &impl_);
		if (res != S_OK)
			throw std::system_error{ windows::make_error_code(res) };
	}

}
