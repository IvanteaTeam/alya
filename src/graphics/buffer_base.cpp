#include<alya/graphics/buffer_base.hpp>
#include<alya/graphics/details/debug.hpp>
#include<alya/graphics/details/d3d11/usage_traits.hpp>

namespace alya::graphics
{


	buffer_base::buffer_base(
		const void*data, 
		size_t size,
		memory_qualifier Q,
		buffer_binding bind,
		d3d11::device_ptr device,
		d3d11::device_context_ptr ctx
	)
		: object_base(device, ctx), size_(size)
	{
		auto [usage, cpu_access] = d3d11::convert_usage(Q);
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = size;
		desc.BindFlags = static_cast<D3D11_BIND_FLAG>(bind);
		desc.CPUAccessFlags = static_cast<D3D11_CPU_ACCESS_FLAG>(cpu_access);
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = static_cast<D3D11_USAGE>(usage);

		D3D11_SUBRESOURCE_DATA init = {};
		init.pSysMem = data;
		init.SysMemPitch = 0;
		init.SysMemSlicePitch = 0;
		ALYA_GFX_CALL(get_device()->CreateBuffer(&desc, data ? &init : nullptr, &buffer));
	}
	
	buffer_map_base::buffer_map_base(
		details::map_type type,
		d3d11::buffer_ptr buffer,
		d3d11::device_ptr device,
		d3d11::device_context_ptr ctx
	) : object_base(device, ctx), buffer(buffer)
	{
		D3D11_MAPPED_SUBRESOURCE res{};

		ALYA_GFX_CALL(get_device_context()->Map(buffer.get(), 0, static_cast<D3D11_MAP>(type), 0, &res));
		
		D3D11_BUFFER_DESC desc{};

		buffer->GetDesc(&desc);

		ptr = reinterpret_cast<char*>(res.pData);
		size_ = desc.ByteWidth;

	}

	buffer_map_base::~buffer_map_base()
	{
		if(buffer)
			ALYA_GFX_CALL(get_device_context()->Unmap(buffer.get(), 0));
	}


}
