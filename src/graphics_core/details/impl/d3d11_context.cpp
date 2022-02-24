#include<alya/graphics/core/details/impl/d3d11_context.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_context::d3d11_context(native_window_handle_type window, pixel_type back_buffer_pixel, pixel_type depth_buffer_pixel, size_t samples) : 
		device_(), 
		default_frame_buffer_(window, back_buffer_pixel, depth_buffer_pixel, samples, device_.native_handle())
	{

		device_.native_handle()->GetImmediateContext(&context_);

	}

	void d3d11_context::set_viewport(int x, int y, int w, int h)
	{
		D3D11_VIEWPORT vp{};
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = x;
		vp.TopLeftY = y;
		vp.Width = w;
		vp.Height = h;
		ALYA_GFX_CALL(context_->RSSetViewports(1, &vp));
	}

	void d3d11_context::draw(size_t from, size_t count)
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

			ALYA_GFX_CALL(device().native_handle()->CreateRasterizerState(&rd, rs.address()));

			context_->RSSetState(rs.get());
		}
		context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ALYA_GFX_CALL(context_->Draw(count, from));
	}

	void d3d11_context::bind_frame_buffer(d3d11_frame_buffer& frame_buffer)
	{
		context_->OMSetRenderTargets(frame_buffer.render_buffers_.size(), reinterpret_cast<ID3D11RenderTargetView**>(frame_buffer.render_buffers_.data()), frame_buffer.depth_stencil_buffer_.get());
	}

	void d3d11_context::bind_sampler(pixel_shader_tag_t, const d3d11_sampler& sampler, size_t slot)
	{
		ALYA_GFX_CALL(context_->PSSetSamplers(slot, 1, sampler.native_handle().address()));
	}

	void d3d11_context::bind_texture(pixel_shader_tag_t, const d3d11_shader_resource_view&view, size_t slot)
	{
		ALYA_GFX_CALL(context_->PSSetShaderResources(slot, 1, view.native_handle().address()));
	}

	void d3d11_context::bind_vertex_shader(const d3d11_vertex_shader&shader)
	{
		ALYA_GFX_CALL(context_->VSSetShader(shader.native_handle().get(), nullptr, 0));
	}

	void d3d11_context::bind_pixel_shader(const d3d11_pixel_shader& shader)
	{
		ALYA_GFX_CALL(context_->PSSetShader(shader.native_handle().get(), nullptr, 0));
	}

	void d3d11_context::bind_constants(vertex_shader_tag_t, const d3d11_buffer& buffer, size_t slot)
	{
		ALYA_GFX_CALL(context_->VSSetConstantBuffers(slot, 1, buffer.native_handle().address()));
	}

	void d3d11_context::bind_constants(pixel_shader_tag_t, const d3d11_buffer& buffer, size_t slot)
	{
		ALYA_GFX_CALL(context_->PSSetConstantBuffers(slot, 1, buffer.native_handle().address()));
	}

	void d3d11_context::do_bind_vertices(const ID3D11Buffer* const*buffers, const uint32_t* strides, const uint32_t* offsets, size_t count)
	{
		ALYA_GFX_CALL(context_->IASetVertexBuffers(0, count, const_cast<ID3D11Buffer**>(buffers), strides, offsets));
	}

	void d3d11_context::bind_input_layout(const ID3D11InputLayout*input_layout)
	{
		ALYA_GFX_CALL(context_->IASetInputLayout(const_cast<ID3D11InputLayout*>(input_layout)));
	}

}
