#pragma once
#include<alya/graphics/core/memory_qualifier.hpp>
#include<alya/utility/bitmask.hpp>
#include<memory>
#include<alya/graphics/core/details/impl/d3d11_buffer.hpp>
#include<alya/graphics/core/context_base.hpp>

namespace alya::graphics::core
{

	class buffer_base;
	class context_base;
	
	class buffer_map_base
	{
	public:

		size_t size()const noexcept
		{
			return size_;
		}

		void* data()const noexcept
		{
			return impl_.get();
		}

	private:

		using impl_t = std::unique_ptr<void, details::d3d11_buffer::unmap_t>;

		buffer_map_base(impl_t impl, size_t size) noexcept : impl_(std::move(impl)), size_(size) {}

		impl_t impl_;
		size_t size_;

		friend class buffer_base;
	};

	class buffer_base
	{
	public:

		buffer_base(const void*init, size_t size, memory_qualifier memory, buffer_binding bind, context_base& context)
			: impl_(init, size, memory, bind, context.impl_)
		{}

		size_t size()const noexcept
		{
			return impl_.size();
		}

	protected:

		buffer_map_base map(details::map_type type)
		{
			return { impl_.map(type), size() };
		}

	private:

		details::d3d11_buffer impl_;

		template<typename, typename, size_t>
		friend class basic_context;
		friend class attribute_stream_base;
	};

}
