#pragma once
#include<d3d11.h>
#include<alya/utility/windows/com/shared_ptr.hpp>



namespace alya::graphics::d3d11
{

	using device_t = ID3D11Device;
	using device_context_t = ID3D11DeviceContext;

	using device_ptr = windows::com::shared_ptr<device_t>;
	using device_context_ptr = windows::com::shared_ptr<device_context_t>;

	using buffer_t = ID3D11Buffer;
	using buffer_ptr = windows::com::shared_ptr<buffer_t>;
	
	using texture2d_t = ID3D11Texture2D;
	using texture2d_ptr = windows::com::shared_ptr<texture2d_t>;

	using srv_t = ID3D11ShaderResourceView;
	using srv_ptr = windows::com::shared_ptr<srv_t>;

	using rtv_t = ID3D11RenderTargetView;
	using rtv_ptr = windows::com::shared_ptr<rtv_t>;

	using dsv_t = ID3D11DepthStencilView;
	using dsv_ptr = windows::com::shared_ptr<dsv_t>;

	using sampler_state_t = ID3D11SamplerState;
	using sampler_state_ptr = windows::com::shared_ptr<sampler_state_t>;
	
	using vertex_shader_t = ID3D11VertexShader;
	using pixel_shader_t = ID3D11PixelShader;

	using vertex_shader_ptr = windows::com::shared_ptr<vertex_shader_t>;
	using pixel_shader_ptr = windows::com::shared_ptr<pixel_shader_t>;

	using input_layout_t = ID3D11InputLayout;
	using input_layout_ptr = windows::com::shared_ptr<input_layout_t>;

}
