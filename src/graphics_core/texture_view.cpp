#include<alya/graphics/core/texture_view.hpp>
#include<alya/utility/windows/win32_error.hpp>

namespace alya::graphics::core
{

	texture_view::texture_view(texture2d_base&t, size_t first, size_t count)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = first;
		desc.Texture2D.MipLevels = count;

		auto res = t.get_device()->CreateShaderResourceView(t.texture.get(), &desc, &srv);
		if (res != S_OK)
			throw std::system_error{windows::make_error_code(res)};
	}

	texture_view::texture_view(texture2d_base& t)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;

		auto res = t.get_device()->CreateShaderResourceView(t.texture.get(), &desc, &srv);
		if (res != S_OK)
			throw std::system_error{windows::make_error_code(res)};
	}

}
