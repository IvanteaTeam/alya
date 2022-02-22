#pragma once
#include<alya/graphics/core/buffer_base.hpp>

namespace alya::graphics::core
{

	class attribute_stream_base
	{
	public:
		attribute_stream_base() = default;
	protected:
		inline attribute_stream_base(const buffer_base& buffer, size_t offset, size_t stride)
			: buffer(buffer.impl_.native_handle()), offset(offset), stride(stride)
		{}
		mutable d3d11::buffer_ptr buffer;
		size_t offset, stride;
		friend class vertex_stream_base;
		//friend class context;
	};

}
