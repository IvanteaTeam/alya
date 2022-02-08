#include<alya/graphics/sampler.hpp>
#include<alya/graphics/details/debug.hpp>

namespace alya
{

	namespace graphics
	{

		sampler::sampler(d3d11::device_ptr device, d3d11::device_context_ptr ctx)
			: object_base(device, ctx)
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
			

			ALYA_GFX_CALL(get_device()->CreateSamplerState(&desc, &state));
		}

	}

}
