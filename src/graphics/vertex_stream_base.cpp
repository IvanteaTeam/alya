#pragma once
#include<alya/graphics/vertex_stream_base.hpp>
#include<alya/graphics/details/debug.hpp>
//#include<alya/utility/windows/error.hpp>

namespace alya::graphics
{

	vertex_stream_base::vertex_stream_base(
		const std::vector<std::pair<attribute_semantic, dxgi::format>>&sem,
		const std::vector<char>&bytecode,
		d3d11::device_ptr device, 
		d3d11::device_context_ptr ctx
	)
		: object_base(device, ctx), attributes(sem.size())
	{

		std::vector<D3D11_INPUT_ELEMENT_DESC> descs;

		size_t slot = 0;
		for (auto [s, t] : sem)
		{
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.Format = static_cast<DXGI_FORMAT>(t);
			desc.SemanticName = s.name.data();
			desc.SemanticIndex = s.index;
			desc.InputSlot = slot++;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.AlignedByteOffset = 0;// D3D11_APPEND_ALIGNED_ELEMENT;
			desc.InstanceDataStepRate = 0;
			descs.push_back(desc);
		}

		//_ALYA_DEBUG_REPORT(int(descs[0].Format));

		ALYA_GFX_CALL(get_device()->CreateInputLayout(descs.data(), descs.size(), bytecode.data(), bytecode.size(), &layout));

	}

	void vertex_stream_base::set(size_t i, const attribute_stream_base&a)
	{
		attributes[i] = a;
		d3d11::input_layout_ptr l;
		ALYA_GFX_CALL(get_device_context()->IAGetInputLayout(&l));
		if (l == layout)
			bind();
	}

	void vertex_stream_base::bind()const
	{
		std::vector<UINT> strides, offsets;
		std::vector<ID3D11Buffer*> buffers;

		for (auto& a : attributes)
		{
			strides.push_back(a.stride);
			offsets.push_back(a.offset);
			buffers.push_back(a.buffer.get());
		}

		ALYA_GFX_CALL(get_device_context()->IASetVertexBuffers(0, buffers.size(), buffers.data(), strides.data(), offsets.data()));
	}

}
