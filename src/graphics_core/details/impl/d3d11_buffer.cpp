#include<alya/graphics/core/details/impl/d3d11_buffer.hpp>
#include<alya/graphics/core/details/impl/d3d11_usage.hpp>
#include<alya/graphics/core/details/impl/d3d11_context.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_buffer::d3d11_buffer(const void*data, size_t size, memory_qualifier memory, buffer_binding bind, d3d11_device&device)
		: size_(size)
	{
		auto [usage, cpu_access] = d3d11_usage(memory);
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = size;
		desc.BindFlags = static_cast<D3D11_BIND_FLAG>(bind);
		desc.CPUAccessFlags = cpu_access;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = usage;

		D3D11_SUBRESOURCE_DATA init = {};
		init.pSysMem = data;
		init.SysMemPitch = 0;
		init.SysMemSlicePitch = 0;
		ALYA_GFX_CALL(device.native_handle()->CreateBuffer(&desc, data ? &init : nullptr, &impl_));
	}

	std::unique_ptr<void, d3d11_buffer::unmap_t> d3d11_buffer::map(details::map_type type)
	{
		using enum details::map_type;
		D3D11_MAP m = D3D11_MAP(0);
		switch (type)
		{
		case read:
			m = D3D11_MAP_READ;
			break;
		case write:
			m = D3D11_MAP_READ;
			break;
		case read_write:
			m = D3D11_MAP_READ_WRITE;
			break;
		case write_discard:
			m = D3D11_MAP_WRITE_DISCARD;
			break;
		}
		windows::com::shared_ptr<ID3D11Device> device;
		impl_->GetDevice(&device);
		windows::com::shared_ptr<ID3D11DeviceContext> context;
		device->GetImmediateContext(&context);
		D3D11_MAPPED_SUBRESOURCE res{};
		ALYA_GFX_CALL(context->Map(impl_.get(), 0, m, 0, &res));
		return std::unique_ptr<void, unmap_t>(res.pData, { impl_ });
	}

	void d3d11_buffer::unmap_t::operator()(void*)noexcept
	{
		if (buffer)
		{
			windows::com::shared_ptr<ID3D11Device> device;
			buffer->GetDevice(&device);
			windows::com::shared_ptr<ID3D11DeviceContext> context;
			device->GetImmediateContext(&context);
			ALYA_GFX_CALL(context->Unmap(buffer.get(), 0));
		}
	}

}
