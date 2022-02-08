#pragma once
#include<alya/graphics/core/basic_buffer.hpp>
#include<alya/graphics/core/attribute_stream.hpp>

namespace alya::graphics::core
{

	template<typename T>
	concept structure_bindable = requires()
	{
		std::tuple_size<T>::value;
	};

	template<typename T, memory_qualifier Q>
	class vertex_buffer;

	template<typename T, memory_qualifier Q>
	class vertex_buffer : public basic_buffer<T, Q, buffer_binding::vertex_buffer>
	{
	public:
		using basic_buffer<T, Q, buffer_binding::vertex_buffer>::basic_buffer;
		template<typename U>
		operator attribute_stream<U>()const
		{
			static_assert(compatible_pixel<pixel_traits<T>, pixel_traits<U>>);

			return attribute_stream<U>(*this, 0, sizeof(T));
		}
	};

	template<typename...T, memory_qualifier Q> 
	class vertex_buffer<std::tuple<T...>, Q> : public basic_buffer<std::tuple<T...>, Q, buffer_binding::vertex_buffer> 
	{
	public:
		using basic_buffer<std::tuple<T...>, Q, buffer_binding::vertex_buffer>::basic_buffer;

		template<size_t I>
		constexpr auto attribute()const
		{
			auto calc_offset = []()constexpr {
				std::tuple<T...>* ptr = nullptr;
				auto el = &std::get<I>(*ptr);
				return reinterpret_cast<const char*>(el) - reinterpret_cast<const char*>(ptr);
			};
			return attribute_stream<std::remove_reference_t<std::remove_all_extents_t<decltype(std::get<I>(std::declval<std::tuple<T...>&>()))>>>(*this, calc_offset(), sizeof(std::tuple<T...>));
		}

	private:

	};


}
