#pragma once
#include<alya/graphics/core/basic_buffer.hpp>
#include<alya/graphics/core/attribute_stream_base.hpp>

namespace alya::graphics::core
{

	template<typename T>
	class attribute_stream : public attribute_stream_base
	{
	public:
		
		attribute_stream() = default;

	private:

		attribute_stream(const buffer_base& buffer, size_t offset, size_t stride)
			: attribute_stream_base(buffer, offset, stride)
		{}

		template<typename, memory_qualifier>
		friend class vertex_buffer;
	};

}
