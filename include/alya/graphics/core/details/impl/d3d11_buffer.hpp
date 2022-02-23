#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<alya/graphics/core/details/mipmap_source.hpp>
#include<alya/graphics/core/details/map_type.hpp>
#include<alya/graphics/core/memory_qualifier.hpp>
#include<alya/graphics/core/buffer_binding.hpp>
#include<memory>

struct ID3D11Device;
struct ID3D11Buffer;

namespace alya::graphics::core::details
{
	
	class d3d11_buffer
	{
	public:

		using native_handle_type = windows::com::shared_ptr<ID3D11Buffer>;

		d3d11_buffer(const void* init, size_t size, memory_qualifier memory, buffer_binding bind, windows::com::shared_ptr<ID3D11Device>device);
		d3d11_buffer(const d3d11_buffer&) = delete;
		d3d11_buffer(d3d11_buffer&&) = default;
		d3d11_buffer& operator=(const d3d11_buffer&) = delete;
		d3d11_buffer& operator=(d3d11_buffer&&) = default;

		size_t size()const noexcept
		{
			return size_;
		}

		struct unmap_t
		{
			windows::com::shared_ptr<ID3D11Buffer> buffer;
			void operator()(void*)noexcept;
		};

		std::unique_ptr<void, unmap_t> map(map_type);

		native_handle_type native_handle()const noexcept
		{
			return impl_;
		}

	private:

		windows::com::shared_ptr<ID3D11Buffer> impl_;
		size_t size_;
	};

}
