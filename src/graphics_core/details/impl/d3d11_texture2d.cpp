#pragma once
#include<alya/graphics/core/details/impl/d3d11_texture2d.hpp>
#include<alya/graphics/core/details/impl/d3d11_usage.hpp>
#include<alya/graphics/core/details/debug.hpp>

namespace alya::graphics::core::details
{

	d3d11_texture2d::d3d11_texture2d(
		size_t width,
		size_t height,
		size_t mipmaps,
		size_t layers,
		const details::mipmap_source* init,
		pixel_type pixel,
		memory_qualifier memory,
		texture_binding bind,
		windows::com::shared_ptr<ID3D11Device>device
	) : 
		width_(width),
		height_(height),
		mipmaps_(mipmaps),
		layers_(layers)
	{

		auto [usage, cpu_access] = d3d11_usage(memory);

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Format = static_cast<DXGI_FORMAT>(pixel);
		desc.Width = width;
		desc.Height = height;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.MipLevels = mipmaps;
		desc.Usage = usage;
		desc.CPUAccessFlags = cpu_access;
		desc.BindFlags = static_cast<D3D11_BIND_FLAG>(bind);
		desc.ArraySize = layers;
		desc.MiscFlags = mipmaps == 0 ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		D3D11_SUBRESOURCE_DATA data[20] = {};

		if (init)
		{
			for (size_t i = 0; i < mipmaps; i++)
			{
				data[i].pSysMem = init[i].data;
				data[i].SysMemPitch = init[i].row_pitch;
				data[i].SysMemSlicePitch = init[i].slice_pitch;
			}
		}

		ALYA_GFX_CALL(device->CreateTexture2D(&desc, init ? data : nullptr, &impl_));

		if (mipmaps == 0)
		{
			ALYA_GFX_CALL(device->CreateShaderResourceView(impl_.get(), nullptr, &generate_mipmaps_view_));
			size_t d = width > height ? width : height;
			while (d)
			{
				mipmaps_++;
				d /= 2;
			}
		}
		
	}

	void d3d11_texture2d::update(size_t mipmap, size_t layer, details::mipmap_source data)noexcept
	{
		size_t index = D3D11CalcSubresource(mipmap, layer, mipmaps());
		windows::com::shared_ptr<ID3D11Device> device;
		impl_->GetDevice(&device);
		windows::com::shared_ptr<ID3D11DeviceContext> context;
		device->GetImmediateContext(&context);
		context->UpdateSubresource(impl_.get(), index, nullptr, data.data, data.row_pitch, data.slice_pitch);
	}

	void d3d11_texture2d::generate_mipmaps()noexcept
	{
		windows::com::shared_ptr<ID3D11Device> device;
		impl_->GetDevice(&device);
		windows::com::shared_ptr<ID3D11DeviceContext> context;
		device->GetImmediateContext(&context);
		context->GenerateMips(generate_mipmaps_view_.get());
	}

}
