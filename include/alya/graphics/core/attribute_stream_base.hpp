#pragma once
#include<alya/graphics/core/buffer_base.hpp>

namespace alya::graphics::core
{

	class attribute_stream_base
	{
	public:
		attribute_stream_base() = default;
		attribute_stream_base(const attribute_stream_base&) = default;
		attribute_stream_base(attribute_stream_base&&) = default;
		attribute_stream_base& operator=(const attribute_stream_base&) = default;
		attribute_stream_base& operator=(attribute_stream_base&&) = default;
	
	protected:
		
		attribute_stream_base(const buffer_base& buffer, size_t offset, size_t stride) noexcept :  
			buffer_(buffer.impl_.native_handle()), offset_(offset), stride_(stride)
		{}
	private:

		details::d3d11_buffer::native_handle_type buffer_;
		size_t offset_, stride_;
		template<typename...T>
		friend class vertex_stream;
		friend class context_base;
	};

}
