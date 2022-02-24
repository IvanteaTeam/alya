#include<alya/graphics/core/context_base.hpp>
#include<alya/utility/windows/com/intialize.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<alya/graphics/core/details/impl/dxgi_format.hpp>

#undef max

namespace alya::graphics::core
{
	/*
	context_base::context_base(HWND hwnd, details::pixel_type color, details::pixel_type depth, size_t samples)
		: samples(samples), color(color), depth(depth), hwnd(hwnd)
	{
		windows::com::initialize();

		D3D_FEATURE_LEVEL feature = D3D_FEATURE_LEVEL_10_0;
		D3D_DRIVER_TYPE driver = D3D_DRIVER_TYPE_HARDWARE;

		int flags = 0;

#ifndef NDEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		ALYA_GFX_CALL(D3D11CreateDevice(nullptr, driver, nullptr, flags, &feature, 1, D3D11_SDK_VERSION, &device, nullptr, &device_context));

		default_frame_buffer_.emplace(details::d3d11_default_frame_buffer(hwnd, color, depth, samples, device));

	}
	void context_base::present(bool sync)
	{

		default_frame_buffer_->present(sync);

	}
	
	void context_base::bind_frame_buffer(details::d3d11_frame_buffer& frame_buffer)
	{
		std::array<ID3D11RenderTargetView*, 8> rtvs{};
		std::transform(frame_buffer.render_buffers_.begin(), frame_buffer.render_buffers_.end(), rtvs.begin(), [](auto& view) { return view.get(); });
		ALYA_GFX_CALL(device_context->OMSetRenderTargets(rtvs.size(), rtvs.data(), frame_buffer.depth_stencil_buffer_.get()));
	}

	void context_base::clear()
	{
		ALYA_GFX_CALL(device_context->ClearState());
	}
	/*
	void context_base::bind_vertices(const vertex_stream_base&vs, const ID3D11Buffer*const*buffers,const uint32_t*offsets, const uint32_t*strides, size_t count)
	{
		ALYA_GFX_CALL(device_context->IASetInputLayout(vs.input_layout_.native_handle().get()));
		ALYA_GFX_CALL(device_context->IASetVertexBuffers(0, count, const_cast<ID3D11Buffer*const*>(buffers), strides, offsets));
	}
	*//*
	void context_base::bind_vertices(std::nullopt_t)
	{
		ALYA_GFX_CALL(device_context->IASetInputLayout(nullptr));
	}

	void context_base::bind_sampler(pixel_shader_target_t, const sampler&s, size_t slot)
	{
		ALYA_GFX_CALL(device_context->PSSetSamplers(slot, 1, s.impl_.native_handle().address()));
	}

	void context_base::bind_vertex_shader(const vertex_shader&vs)
	{
		ALYA_GFX_CALL(device_context->VSSetShader(vs.impl_.native_handle().get(), nullptr, 0));
	}

	void context_base::bind_pixel_shader(const pixel_shader&ps)
	{
		ALYA_GFX_CALL(device_context->PSSetShader(ps.impl_.native_handle().get(), nullptr, 0));
	}

	void context_base::bind_constants(vertex_shader_target_t, const buffer_base& buffer, size_t slot)
	{
		ALYA_GFX_CALL(device_context->VSSetConstantBuffers(slot, 1, buffer.impl_.native_handle().address()));
	}

	void context_base::bind_constants(pixel_shader_target_t, const buffer_base& buffer, size_t slot)
	{
		ALYA_GFX_CALL(device_context->PSSetConstantBuffers(slot, 1, buffer.impl_.native_handle().address()));
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
		default_frame_buffer_->clear_depth(d);
	}

	void context_base::clear_color(float r, float g, float b, float a)
	{
		default_frame_buffer_->clear_color(r, g, b, a, 0);
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
		device_context->PSSetShaderResources(slot, 1, view.impl_.native_handle().address());
	}*/

}
