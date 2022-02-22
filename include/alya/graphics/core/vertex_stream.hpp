#pragma once
#include<alya/graphics/core/vertex_stream_base.hpp>

namespace alya::graphics::core
{


	template<typename...T>
	class vertex_stream : public vertex_stream_base
	{
	public:

		template<typename S, typename C>
		vertex_stream(const std::array<attribute_semantic, sizeof...(T)>&sem, const S& shader, C& ctx)
			: vertex_stream_base{make_semantic(sem), shader, ctx } 
		{

		}


		template<size_t I>
		void attach(attribute_stream<std::remove_reference_t<std::remove_all_extents_t<decltype(std::get<I>(std::declval<std::tuple<T...>&>()))>>> a)
		{
			using R = std::remove_reference_t<std::remove_all_extents_t<decltype(std::get<I>(std::declval<std::tuple<T...>&>()))>>;
			vertex_stream_base::set(I, a);
		}
		
		template<size_t I, typename U>
		void attach(attribute_stream<U> a)
		{
			using R = std::remove_reference_t<std::remove_all_extents_t<decltype(std::get<I>(std::declval<std::tuple<T...>&>()))>>;
			static_assert(compatible_pixel<pixel_traits<R>, pixel_traits<U>>);
			vertex_stream_base::set(I, a);
		}


	private:

		static inline std::vector<std::pair<attribute_semantic, details::pixel_type>> make_semantic(const std::array<attribute_semantic, sizeof...(T)>&sem)
		{
			return ([&]<size_t...I>(std::index_sequence<I...>) {
				std::array<details::pixel_type, sizeof...(T)> arr = { details::make_pixel_type<T>()... };
					

				return std::vector<std::pair<attribute_semantic, details::pixel_type>>{std::pair{ sem[I], arr[I] }... };
			})(std::make_index_sequence<sizeof...(T)>{});
		}

	};

}
