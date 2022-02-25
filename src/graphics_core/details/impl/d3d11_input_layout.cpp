#include<alya/graphics/core/details/impl/d3d11_input_layout.hpp>
#include<alya/graphics/core/details/impl/dxgi_format.hpp>
#include<alya/graphics/core/details/impl/d3d11_context.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>
#include<vector>

namespace alya::graphics::core::details
{

	d3d11_input_layout::d3d11_input_layout(std::initializer_list<attribute_signature>signatures, const void* bytecode, size_t bytecode_size, d3d11_context&context)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> descs;

		size_t slot = 0;
		for (auto [type, name, index] : signatures)
		{
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.Format = details::dxgi_format(type);
			desc.SemanticName = name;
			desc.SemanticIndex = index;
			desc.InputSlot = slot++;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.AlignedByteOffset = 0;// D3D11_APPEND_ALIGNED_ELEMENT;
			desc.InstanceDataStepRate = 0;
			descs.push_back(desc);
		}

		//_ALYA_DEBUG_REPORT(int(descs[0].Format));

		ALYA_GFX_CALL(context.device().native_handle()->CreateInputLayout(descs.data(), descs.size(), bytecode, bytecode_size, &impl_));
	}

}
