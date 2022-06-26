#pragma once
#include<type_traits>
#include<tuple>
#include<array>
#include<alya/utility/normalized.hpp>

namespace alya
{
	
	template<typename T>
	class pixel_traits
	{
	public:

		using value_type = T;
		using element_type = std::remove_reference_t<decltype(std::declval<value_type&>()[0])>;
		static const size_t element_count = std::tuple_size<value_type>::value;

		constexpr pixel_traits() = default;

	};

	template<typename T, typename U>
	concept compatible_pixel = requires
	{
		std::tuple_size<typename T::value_type>::value;
		std::same_as<typename T::element_type, typename U::element_type>;
	} || std::same_as<typename T::value_type, typename U::value_type>;

	template<typename T, typename U>
	concept convertable_pixel =
		is_normalized<typename T::element_type>::value && (
			std::floating_point<typename U::element_type> ||
			is_normalized<typename U::element_type>::value && (
				is_signed<typename T::element_type>::value && is_signed<typename U::value_type>::value ||
				!is_signed<typename T::element_type>::value && !is_signed<typename U::value_type>::value
			)
		)
		|| !is_normalized<typename T::element_type>::value && !is_normalized<typename U::element_type>(
			is_signed<typename T::element_type>::value && is_signed<typename U::value_type>::value ||
			!is_signed<typename T::element_type>::value && !is_signed<typename U::value_type>::value
		)
		|| std::floating_point<typename T::element_type> && is_normalized<typename U::element_type>::value 
		|| std::floating_point<typename T::element_type> && std::floating_point<typename U::element_type>;

	template<typename F, typename U>
	constexpr auto pixel_cast(const U&a)noexcept
	{
		using A = std::remove_cvref_t<U>;
		
		using B = std::remove_cvref_t<std::remove_pointer_t<A>>;
		using C = std::remove_cvref_t<std::remove_pointer_t<F>>;

		if constexpr (std::is_pointer_v<A>)
		{	
			static_assert(std::is_pointer_v<F>);
			static_assert(compatible_pixel<pixel_traits<B>, pixel_traits<C>>);
			return reinterpret_cast<F>(a);
		}
		else
		{
			static_assert(convertable_pixel<pixel_traits<B>, pixel_traits<C>>);

			C out;
			using T = pixel_traits<C>;
			
			for (size_t i = 0; i < T::element_count; i++)
			{
				auto default_val = [&]() constexpr
				{
					if constexpr (std::floating_point<typename T::element_type> || is_normalized<typename T::element_type>::value)
						if (i <= 2)
							return 0.0;
						else
							return 1.0;
					else
						return 0;
				};
				if (i < pixel_traits<B>::element_count)
					out[i] = a[i];
				else
					out[i] = typename T::element_type(default_val());
			}

			return out;
		}
	}
	

	using rgb = std::array<uint8n_t, 3>;
	using rgba = std::array<uint8n_t, 4>;

}
