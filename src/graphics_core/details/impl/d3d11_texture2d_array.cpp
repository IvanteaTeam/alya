#include<alya/graphics/core/details/impl/d3d11_texture2d_array.hpp>
#include<alya/graphics/core/details/impl/dxgi_format.hpp>
#include<alya/graphics/core/details/impl/d3d11_usage.hpp>
#include<alya/graphics/core/details/debug.hpp>

namespace alya::graphics::core::details
{

	d3d11_texture2d_array::d3d11_texture2d_array(
		size_t width,
		size_t height,
		size_t mipmaps,
		size_t layers,
		size_t samples,
		bool cube,
		pixel_type pixel,
		memory_qualifier memory,
		texture_binding bind,
		windows::com::shared_ptr<ID3D11Device>device
	) : 
		width_(width),
		height_(height),
		mipmaps_(mipmaps),
		layers_(layers),
		samples_(samples)
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
		desc.MiscFlags = (mipmaps == 0?D3D11_RESOURCE_MISC_GENERATE_MIPS : 0) | (cube?D3D11_RESOURCE_MISC_TEXTURECUBE : 0);

		ALYA_GFX_CALL(device->CreateTexture2D(&desc, nullptr, &impl_));

		if (mipmaps_ == 0)
		{

			ALYA_GFX_CALL(device->CreateShaderResourceView(impl_.get(), nullptr, &generate_mipmaps_view_));

			auto count_mipmaps = [](size_t w, size_t h)constexpr {
				auto next = [](size_t d) constexpr{ return d > 1 ? d / 2 : 1; };
				
				size_t c = 1;

				for (; w != 1 && h != 1; w = next(w), h = next(h))
					c++;
				return c;
			};
			
			mipmaps_ = count_mipmaps(width_, height_);

		}

	}

	d3d11_texture2d_array::d3d11_texture2d_array(
		size_t width,
		size_t height,
		size_t mipmaps,
		size_t layers,
		bool cube,
		const mipmap_source*init,
		pixel_type pixel,
		memory_qualifier memory,
		texture_binding bind,
		windows::com::shared_ptr<ID3D11Device> device
	) : 
		width_(width),
		height_(height),
		mipmaps_(mipmaps),
		layers_(layers),
		samples_(1)
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
		desc.MiscFlags = cube ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

		D3D11_SUBRESOURCE_DATA data[20] = {};

		std::transform(init, init + mipmaps_ * layers_, std::begin(data), 
			[](mipmap_source source) { 
				return D3D11_SUBRESOURCE_DATA{ 
					.pSysMem = source.data, 
					.SysMemPitch = uint32_t(source.row_pitch), 
					.SysMemSlicePitch = uint32_t(source.slice_pitch) 
				}; 
			}
		);

		ALYA_GFX_CALL(device->CreateTexture2D(&desc, data, &impl_));

	}

	void d3d11_texture2d_array::write_mipmap(mipmap_source data, size_t layer, size_t mipmap)
	{
		size_t index = D3D11CalcSubresource(mipmap, layer, mipmaps());
		windows::com::shared_ptr<ID3D11Device> device;
		impl_->GetDevice(&device);
		windows::com::shared_ptr<ID3D11DeviceContext> context;
		device->GetImmediateContext(&context);
		ALYA_GFX_CALL(context->UpdateSubresource(impl_.get(), index, nullptr, data.data, data.row_pitch, data.slice_pitch));
	}

	void d3d11_texture2d_array::generate_mipmaps()
	{
		windows::com::shared_ptr<ID3D11Device> device;
		impl_->GetDevice(&device);
		windows::com::shared_ptr<ID3D11DeviceContext> context;
		device->GetImmediateContext(&context);
		ALYA_GFX_CALL(context->GenerateMips(generate_mipmaps_view_.get()));
	}

}
