#pragma once
#include<alya/graphics/basic_buffer.hpp>
#include<alya/graphics/attribute_stream_base.hpp>

namespace alya::graphics
{

	template<typename T>
	class attribute_stream : public attribute_stream_base
	{
	public:
		
		attribute_stream() = default;
		attribute_stream(const attribute_stream&) = default;
		attribute_stream(attribute_stream&&) = default;
		attribute_stream& operator=(const attribute_stream&) = default;
		attribute_stream& operator=(attribute_stream&&) = default;

	private:

		explicit attribute_stream(const buffer_base& buffer, size_t offset, size_t stride)
			: attribute_stream_base(buffer, offset, stride)
		{}
		template<typename, memory_qualifier>
		friend class vertex_buffer;
	};

}
