#pragma once
#include<alya/graphics/core/context_base.hpp>
#include<alya/graphics/core/attribute_stream.hpp>
#include<alya/graphics/core/details/impl/d3d11_vertex_stream.hpp>
#include<alya/graphics/core/vertex_shader.hpp>

namespace alya::graphics::core
{

	
	struct attribute_semantic
	{
		const char* name;
		size_t index;
	};

	template<typename...T>
	class vertex_stream// : public vertex_stream_base
	{
	public:

		vertex_stream(const std::array<attribute_semantic, sizeof...(T)>&sematics, const vertex_shader&shader, context_base&context)
			: vertex_stream(std::make_index_sequence<sizeof...(T)>{}, sematics, shader, context)
		{}

		template<size_t I>
		void attach(attribute_stream<std::tuple_element_t<I, std::tuple<T...>>> a)
		{
			attach(std::move(a), I);
		}
		
		template<size_t I, typename U>
		void attach(attribute_stream<U> a) requires compatible_pixel<pixel_traits<U>, pixel_traits<std::tuple_element_t<I, std::tuple<T...>>>>
		{
			attach(std::move(a), I);
		}

	private:

		void attach(attribute_stream_base&& stream, size_t index)
		{
			impl_.attach_attribute_stream(stream.buffer_, stream.offset_, stream.stride_, index);
		}

		template<size_t...I>
		vertex_stream(std::index_sequence<I...>, const std::array<attribute_semantic, sizeof...(I)>&semantic, const vertex_shader&shader, context_base&context)
			: impl_({ details::attribute_signature{details::make_pixel_type<std::tuple_element_t<I, std::tuple<T...>>>(), semantic[I].name, semantic[I].index }... }, shader.impl_, context.impl_.device().native_handle())
		{}

		details::d3d11_vertex_stream<sizeof...(T)> impl_;

		template<typename, typename, size_t>
		friend class basic_context;
	};

}
