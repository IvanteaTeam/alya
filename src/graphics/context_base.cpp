#include<alya/graphics/context_base.hpp>
#include<alya/utility/windows/com/intialize.hpp>
#include<alya/graphics/details/debug.hpp>
#include<alya/utility/windows/win32_error.hpp>

#undef max

namespace alya::graphics
{

	context_base::context_base(HWND hwnd, dxgi::format color, dxgi::format depth, size_t samples)
		: current_frame_buffer(nullptr), samples(samples), color(color), depth(depth), hwnd(hwnd)
	{
		windows::com::initialize();

		D3D_FEATURE_LEVEL feature = D3D_FEATURE_LEVEL_10_0;
		D3D_DRIVER_TYPE driver = D3D_DRIVER_TYPE_HARDWARE;

		int flags = 0;

#ifndef NDEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		ALYA_GFX_CALL(D3D11CreateDevice(nullptr, driver, nullptr, flags, &feature, 1, D3D11_SDK_VERSION, &device, nullptr, &device_context));


		//ALYA_WIN32_CALL(ALYA_GFX_CALL(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory))));

		{
			windows::com::shared_ptr<IDXGIDevice> d = device.query_interface<IDXGIDevice>();
			windows::com::shared_ptr<IDXGIAdapter> adapter;
			d->GetParent(__uuidof(*adapter.get()), reinterpret_cast<void**>(&adapter));
			adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
		}

		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferCount = 3;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		sd.BufferDesc.Format = static_cast<DXGI_FORMAT>(color);
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = true;


		ALYA_GFX_CALL(factory->CreateSwapChain(device.get(), &sd, &swap_chain));

		RECT ca_size;
		GetClientRect(hwnd, &ca_size);
		width = ca_size.right - ca_size.left;
		height = ca_size.bottom - ca_size.top;
		resize_buffers(ca_size.right - ca_size.left, ca_size.bottom - ca_size.top);
		
	}

	void context_base::resize_buffers(size_t w, size_t h)
	{
		w = std::max(w, 8ull);
		h = std::max(h, 8ull);

		back_buffer = nullptr;
		back_buffer_ms = nullptr;
		depth_buffer = nullptr;

		ALYA_GFX_CALL(swap_chain->ResizeBuffers(3, w, h, static_cast<DXGI_FORMAT>(color), 0));

		windows::com::shared_ptr<ID3D11Texture2D> texture;

		ALYA_GFX_CALL(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture)));

		ALYA_GFX_CALL(device->CreateRenderTargetView(texture.get(), nullptr, &back_buffer));


		if (samples > 1){
			windows::com::shared_ptr<ID3D11Texture2D> ms;
		
			D3D11_TEXTURE2D_DESC desc = {};
			
			desc.Format = static_cast<DXGI_FORMAT>(color);
			desc.ArraySize = 1;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.SampleDesc.Count = samples;
			desc.SampleDesc.Quality = 0;
			desc.Height = h;
			desc.Width = w;
			desc.MipLevels = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			
			ALYA_GFX_CALL(device->CreateTexture2D(&desc, nullptr, &ms));

			ALYA_GFX_CALL(device->CreateRenderTargetView(ms.get(), nullptr, &back_buffer_ms));

		}

		if(depth != dxgi::format::none)
		{
			windows::com::shared_ptr<ID3D11Texture2D> db;
			D3D11_TEXTURE2D_DESC desc = {};

			desc.Format = static_cast<DXGI_FORMAT>(depth);
			desc.ArraySize = 1;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.SampleDesc.Count = samples;
			desc.SampleDesc.Quality = 0;
			desc.Height = h;
			desc.Width = w;
			desc.MipLevels = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;

			ALYA_GFX_CALL(device->CreateTexture2D(&desc, nullptr, &db));

			ALYA_GFX_CALL(device->CreateDepthStencilView(db.get(), nullptr, &depth_buffer));
		}

	}

	void context_base::present(bool sync)
	{
		if (back_buffer_ms)
		{
			windows::com::shared_ptr<ID3D11Resource> bb, bbms;
			back_buffer->GetResource(&bb);
			back_buffer_ms->GetResource(&bbms);
			ALYA_GFX_CALL(device_context->ResolveSubresource(bb.get(), 0, bbms.get(), 0, static_cast<DXGI_FORMAT>(color)));
			device_context->Flush();
		}
		auto ret = swap_chain->Present(sync ? 1 : 0, 0);

		if (ret != S_OK && ret != DXGI_STATUS_OCCLUDED)
		{
			//ALYA_WIN32_CALL(ret);
			throw std::system_error{ windows::make_error_code(ret) };
		}
		else
		{
			RECT ca;

			GetClientRect(hwnd, &ca);

			size_t w = ca.right - ca.left, h = ca.bottom - ca.top;

			if (!(w == width && h == height))
			{
				width = w;
				height = h;
				resize_buffers(w, h);
			}
		}

	}
	
	void context_base::bind_frame_buffer(frame_buffer_base& fb)
	{
		fb.bind();
	}

	void context_base::bind_frame_buffer(default_frame_buffer_t)
	{
		current_frame_buffer = nullptr;
		std::array<d3d11::rtv_t*, 8> rtvs{};
		rtvs[0] = back_buffer_ms ? back_buffer_ms.get() : back_buffer.get();
		ALYA_GFX_CALL(device_context->OMSetRenderTargets(rtvs.size(), rtvs.data(), depth_buffer ? depth_buffer.get() : nullptr));
	}

	void context_base::bind_frame_buffer(std::nullopt_t)
	{
		ALYA_GFX_CALL(device_context->OMSetRenderTargets(0, nullptr, nullptr));
	}

	void context_base::clear()
	{
		ALYA_GFX_CALL(device_context->ClearState());
	}

	void context_base::bind_vertices(const vertex_stream_base&vs)
	{
		ALYA_GFX_CALL(device_context->IASetInputLayout(vs.layout.get()));
		vs.bind();
	}

	void context_base::bind_vertices(std::nullopt_t)
	{
		ALYA_GFX_CALL(device_context->IASetInputLayout(nullptr));
	}

	void context_base::bind_sampler(pixel_shader_target_t, const sampler&s, size_t slot)
	{
		ALYA_GFX_CALL(device_context->PSSetSamplers(slot, 1, s.state.address()));
	}

	void context_base::bind_vertex_shader(const vertex_shader&vs)
	{
		ALYA_GFX_CALL(device_context->VSSetShader(vs.shader.get(), nullptr, 0));
	}

	void context_base::bind_pixel_shader(const pixel_shader&ps)
	{
		ALYA_GFX_CALL(device_context->PSSetShader(ps.shader.get(), nullptr, 0));
	}

	void context_base::bind_constants(vertex_shader_target_t, const buffer_base& buffer, size_t slot)
	{
		ALYA_GFX_CALL(device_context->VSSetConstantBuffers(slot, 1, buffer.buffer.address()));
	}

	void context_base::bind_constants(pixel_shader_target_t, const buffer_base& buffer, size_t slot)
	{
		ALYA_GFX_CALL(device_context->PSSetConstantBuffers(slot, 1, buffer.buffer.address()));
	}

	void context_base::set_viewport(int x, int y, int w, int h)
	{
		D3D11_VIEWPORT vp{};
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = x;
		vp.TopLeftY = y;
		vp.Width = w;
		vp.Height = h;
		ALYA_GFX_CALL(device_context->RSSetViewports(1, &vp));
	}

	void context_base::draw(size_t from, size_t count)
	{
		{
			D3D11_RASTERIZER_DESC rd{};

			rd.AntialiasedLineEnable = true;
			rd.CullMode = D3D11_CULL_NONE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.DepthClipEnable = true;
			rd.FillMode = D3D11_FILL_SOLID;
			rd.FrontCounterClockwise = false;
			rd.MultisampleEnable = true;
			rd.ScissorEnable = false;
			rd.SlopeScaledDepthBias = 0;

			windows::com::shared_ptr<ID3D11RasterizerState> rs;

			ALYA_GFX_CALL(device->CreateRasterizerState(&rd, rs.address()));

			device_context->RSSetState(rs.get());
		}
		device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ALYA_GFX_CALL(device_context->Draw(count, from));
	}

	void context_base::clear_depth(float d)
	{
		if (depth_buffer)
			device_context->ClearDepthStencilView(depth_buffer.get(), D3D11_CLEAR_DEPTH, d, 0);
	}

	void context_base::clear_color(float r, float g, float b, float a)
	{
		float color[] = { r, g, b, a };
		device_context->ClearRenderTargetView(back_buffer_ms ? back_buffer_ms.get() : back_buffer.get(), color);
	}

	void context_base::set_depth_stencil_state(const depth_stencil_state&s)
	{
		windows::com::shared_ptr<ID3D11DepthStencilState> dss;

		D3D11_DEPTH_STENCIL_DESC desc{};

		desc.DepthEnable = s.depth_test_enable;
		desc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(s.depth_cmp);
		desc.DepthWriteMask = s.rewrite_depth ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.StencilEnable = s.stencil_enable;
		desc.StencilReadMask = s.stencil_read_mask;
		desc.StencilWriteMask = s.stencil_write_mask;
		desc.FrontFace.StencilFunc = static_cast<D3D11_COMPARISON_FUNC>(s.stencil_cmp);
		desc.FrontFace.StencilFailOp = static_cast<D3D11_STENCIL_OP>(s.stencil_fail);
		desc.FrontFace.StencilDepthFailOp = static_cast<D3D11_STENCIL_OP>(s.stencil_pass_depth_fail);
		desc.FrontFace.StencilPassOp = static_cast<D3D11_STENCIL_OP>(s.stencil_pass_depth_pass);
		desc.BackFace = desc.FrontFace;
	}

	void context_base::bind_texture(pixel_shader_target_t, const texture_view& view, size_t slot)
	{
		device_context->PSSetShaderResources(slot, 1, view.srv.address());
	}

}
