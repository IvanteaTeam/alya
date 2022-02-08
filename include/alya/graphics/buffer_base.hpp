#pragma once
#include<alya/graphics/details/d3d11/object_base.hpp>
#include<alya/graphics/memory_qualifier.hpp>
#include<alya/utility/bitmask.hpp>
//#include<alya/graphics/details/d3d11/map_enum.hpp>
#include<memory>

namespace alya::graphics
{

	class buffer_base;
	class buffer_map_base;

	namespace details
	{

		enum class map_type
		{
			read = D3D11_MAP_READ,
			write = D3D11_MAP_WRITE,
			read_write = D3D11_MAP_READ_WRITE,
			write_discard = D3D11_MAP_WRITE_DISCARD
		};

		inline buffer_map_base map(buffer_base& buffer, map_type);
	}

	enum class buffer_binding
	{
		vertex_buffer = D3D11_BIND_VERTEX_BUFFER,
		index_buffer = D3D11_BIND_INDEX_BUFFER,
		constant_buffer = D3D11_BIND_CONSTANT_BUFFER
	};

	class buffer_map_base : protected d3d11::object_base
	{
	public:

		buffer_map_base(buffer_map_base&&) = default;
		buffer_map_base& operator=(buffer_map_base&&) = default;

		inline void* data()const
		{
			return reinterpret_cast<void*>(ptr);
		}

		constexpr size_t size()const
		{
			return size_;
		}

		~buffer_map_base();

	private:

		buffer_map_base(
			details::map_type,
			d3d11::buffer_ptr buffer,
			d3d11::device_ptr device,
			d3d11::device_context_ptr ctx
		);

		char* ptr;
		size_t size_;
		d3d11::buffer_ptr buffer;
		
		friend buffer_map_base details::map(buffer_base&, details::map_type);
	};

	class buffer_base : protected d3d11::object_base
	{
	public:

		

		template<typename Context, typename T>
		buffer_base(
			const T* data, 
			size_t count,
			memory_qualifier Q,
			buffer_binding bind,
			Context& ctx
		) : buffer_base(
			data, 
			count * sizeof(T),
			Q,
			bind,
			get_device(ctx), 
			get_device_context(ctx)
		) {}

		constexpr size_t size()const
		{
			return size_;
		}

	protected:
	

	private:

		buffer_base(
			const void* data,
			size_t size,
			memory_qualifier Q,
			buffer_binding bind,
			d3d11::device_ptr,
			d3d11::device_context_ptr
		);



		size_t size_;
		mutable d3d11::buffer_ptr buffer;
		friend class context_base;
		friend class buffer_map_base;
		friend buffer_map_base details::map(buffer_base&, details::map_type);
		friend class attribute_stream_base;
	};

	namespace details
	{

		buffer_map_base map(buffer_base&buffer, map_type map)
		{
			return buffer_map_base(map, buffer.buffer, buffer.get_device(), buffer.get_device_context());
		}

	}

}


namespace alya
{
	template<>
	struct is_bitmask_enum<graphics::buffer_binding> : std::true_type {};
}
