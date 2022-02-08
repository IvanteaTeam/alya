#include<alya/graphics/texture2d_base.hpp>
#include<alya/graphics/details/debug.hpp>
#include<alya/graphics/details/d3d11/usage_traits.hpp>
#include<alya/utility/windows/win32_error.hpp>

namespace alya::graphics
{
	
	texture2d_base::texture2d_base(
		const texture2d_base::init_t& init,
		size_t width,
		size_t height,
		size_t samples,
		bool generate_mipmaps,
		dxgi::format format,
		memory_qualifier Q,
		texture_binding B,
		d3d11::device_ptr device,
		d3d11::device_context_ptr ctx
	)
		: object_base(device, ctx), width_(width), height_(height)
	{
		auto [mipmaps, slices, levels] = init;

		if (levels == 0)
			levels = mipmaps.size() / slices;

		layers_ = slices;
		mipmaps_ = levels;

		auto [usage, cpu_access] = d3d11::convert_usage(Q);

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Format = static_cast<DXGI_FORMAT>(format);
		desc.Width = width;
		desc.Height = height;
		desc.SampleDesc.Count = samples;
		desc.SampleDesc.Quality = 0;
		desc.MipLevels = generate_mipmaps ? 0 : levels;
		desc.Usage = static_cast<D3D11_USAGE>(usage);
		desc.CPUAccessFlags = static_cast<D3D11_CPU_ACCESS_FLAG>(cpu_access);
		desc.BindFlags = static_cast<D3D11_BIND_FLAG>(B);
		desc.ArraySize = slices;
		desc.MiscFlags = generate_mipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		if (!generate_mipmaps)
		{
			std::vector<D3D11_SUBRESOURCE_DATA> init_data;



			for (auto&[data, align] : mipmaps)
			{
				init_data.push_back(D3D11_SUBRESOURCE_DATA{ .pSysMem = data, .SysMemPitch = static_cast<uint32_t>(align), .SysMemSlicePitch = 0 });
			}
			ALYA_GFX_CALL(get_device()->CreateTexture2D(&desc, init_data.empty() ? nullptr : init_data.data(), &texture));
		}
		else
		{
			ALYA_GFX_CALL(get_device()->CreateTexture2D(&desc, nullptr, &texture));
			
			for (auto [data, align] : mipmaps)
				get_device_context()->UpdateSubresource(texture.get(), D3D11CalcSubresource(0, 0, levels), nullptr, data, align, 0);
			
			windows::com::shared_ptr<ID3D11ShaderResourceView> view;

			auto res = get_device()->CreateShaderResourceView(texture.get(), nullptr, view.address());

			if (res != S_OK)
				throw std::system_error{windows::make_error_code(res)};

			get_device_context()->GenerateMips(view.get());

			auto d = [](size_t x)->size_t { 
				if (x > 1)
					return x / 2;
				else
					return 1;
			};

			mipmaps_ = 1;
			
			while (!(width == 1 && height == 1))
			{
				width = d(width);
				height = d(height);
				mipmaps_++;
			}
		}
		
	}

	size_t texture2d_base::mipmaps()const noexcept
	{
		return mipmaps_;
	}

	size_t texture2d_base::width(size_t i)const noexcept
	{
		return width_ / (1ull << i);
	}

	size_t texture2d_base::height(size_t i)const noexcept
	{
		return height_ / (1ull << i);
	}

	size_t texture2d_base::layers()const noexcept
	{
		return layers_;
	}
	/*
	void clear_depth(texture2d_base&t, float d)
	{
		t.get_device_context()->ClearDepthStencilView(t.dsv.get(), D3D11_CLEAR_DEPTH, d, 0);
	}

	void clear_color(texture2d_base& t, float* c)
	{
		t.get_device_context()->ClearRenderTargetView(t.rtv.get(), c);
	}
	*/
}

