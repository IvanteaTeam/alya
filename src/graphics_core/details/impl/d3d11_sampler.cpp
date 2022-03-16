#include<alya/graphics/core/details/impl/d3d11_sampler.hpp>
#include<alya/graphics/core/details/impl/d3d11_context.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_sampler::d3d11_sampler(d3d11_device&device)
	{
		D3D11_SAMPLER_DESC desc = {};
		desc.Filter = //D3D11_FILTER_MIN_MAG_MIP_POINT;
			D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		//D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0;
		//desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		//desc.BorderColor[0] = 0;
		//desc.BorderColor[1] = 0;
		//desc.BorderColor[2] = 0;
		//desc.BorderColor[3] = 1;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;


		ALYA_GFX_CALL(device.native_handle()->CreateSamplerState(&desc, &impl_));
	}

}
