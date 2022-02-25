#pragma once
#include<alya/graphics/core/details/impl/d3d11_texture2d.hpp>
#include<alya/graphics/core/details/impl/d3d11_usage.hpp>
#include<alya/graphics/core/details/impl/dxgi_format.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<alya/graphics/core/details/impl/d3d11_context.hpp>

namespace alya::graphics::core::details
{
	d3d11_texture2d::d3d11_texture2d(
		size_t width,
		size_t height,
		size_t mipmaps,
		size_t layers,
		size_t samples,
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
		desc.Format = dxgi_format(pixel);
		desc.Width = width;
		desc.Height = height;
		desc.SampleDesc.Count = samples;
		desc.SampleDesc.Quality = 0;
		desc.MipLevels = mipmaps;
		desc.Usage = usage;
		desc.CPUAccessFlags = cpu_access;
		desc.BindFlags = static_cast<D3D11_BIND_FLAG>(bind);
		desc.ArraySize = layers;
		desc.MiscFlags = 0;

		ALYA_GFX_CALL(device->CreateTexture2D(&desc, nullptr, &impl_));

	}

	d3d11_texture2d::d3d11_texture2d(
		size_t width,
		size_t height,
		size_t mipmaps,
		size_t layers,
		size_t samples,
		pixel_type pixel,
		memory_qualifier memory,
		texture_binding bind,
		d3d11_context&context
	) : 
		d3d11_texture2d(
			width,
			height,
			mipmaps,
			layers,
			samples,
			pixel,
			memory,
			bind,
			context.device().native_handle()
		)
	{}
	
	d3d11_texture2d::d3d11_texture2d(
		size_t width,
		size_t height,
		size_t mipmaps,
		size_t layers,
		const details::mipmap_source* init,
		pixel_type pixel,
		memory_qualifier memory,
		texture_binding bind,
		d3d11_context&context
	) : 
		width_(width),
		height_(height),
		mipmaps_(mipmaps),
		layers_(layers)
	{

		auto [usage, cpu_access] = d3d11_usage(memory);

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Format = dxgi_format(pixel);
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

		ALYA_GFX_CALL(context.device().native_handle()->CreateTexture2D(&desc, init ? data : nullptr, &impl_));

		if (mipmaps == 0)
		{
			ALYA_GFX_CALL(context.device().native_handle()->CreateShaderResourceView(impl_.get(), nullptr, &generate_mipmaps_view_));
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
		ALYA_GFX_CALL(context->UpdateSubresource(impl_.get(), index, nullptr, data.data, data.row_pitch, data.slice_pitch));
	}

	void d3d11_texture2d::generate_mipmaps()noexcept
	{
		windows::com::shared_ptr<ID3D11Device> device;
		impl_->GetDevice(&device);
		windows::com::shared_ptr<ID3D11DeviceContext> context;
		device->GetImmediateContext(&context);
		ALYA_GFX_CALL(context->GenerateMips(generate_mipmaps_view_.get()));
	}

}
