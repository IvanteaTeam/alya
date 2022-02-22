#pragma once
#include<alya/graphics/core/basic_buffer.hpp>

namespace alya::graphics::core
{

	template<typename T, size_t>
	struct vector {
		static_assert(sizeof(T) == 4);
	};

	template<typename T, size_t>
	struct matrix {
		static_assert(sizeof(T) == 4);
	};


	
	template<typename T>
	struct constant_buffer_variable_type{
		using type = vector<typename pixel_traits<T>::element_type, pixel_traits<T>::element_count>;
	};

	template<std::integral T>
	struct constant_buffer_variable_type<T>{
		using type = vector<T, 1>;
	};

	template<std::floating_point T>
	struct constant_buffer_variable_type<T>
	{
		using type = vector<T, 1>;
	};


	namespace details
	{

		template<typename T>
		struct align_of;

		template<typename T, size_t N>
		struct align_of<vector<T, N>> : std::integral_constant<size_t, sizeof(T)* N> {};

		template<typename T, size_t N>
		struct align_of<matrix<T, N>> : std::integral_constant<size_t, sizeof(float)* 4> {};

		template<typename T>
		struct size_of;

		template<typename T, size_t N>
		struct size_of<vector<T, N>> : std::integral_constant<size_t, sizeof(T)* N> {};

		template<typename T, size_t N>
		struct size_of<matrix<T, N>> : std::integral_constant<size_t, sizeof(float)* N* N> {};

		template<typename T>
		class constant_buffer_align
		{
		public:
			constant_buffer_align(const T& data)
				: data(convert(data))
			{}

			template<size_t N, typename T>
			static constexpr auto push_back(std::array<int, N>arr, T value)
			{
				std::array<int, N + sizeof(T) / sizeof(int)> arr1;
				auto it = std::copy(arr.begin(), arr.end(), arr1.begin());
				std::copy(value.begin(), value.end(), it);
				return arr1;
			}

			template<size_t I, size_t N>
			static constexpr auto convert2(std::array<int, N>arr)
			{
				if constexpr (I >= std::tuple_size_v<T>)
					return arr;
				else {
					using U1 = decltype(get<I>(std::declval<T&>()));
					using U = std::remove_cv_t<std::remove_reference_t<U1>>;
					constexpr size_t align = align_of<constant_buffer_variable_type<U>::type>::value / 4;
					constexpr size_t size = size_of<constant_buffer_variable_type<U>::type>::value / 4;
					if constexpr (N % 4 <= 4 - align)
						return convert2<I + 1>(push_back(arr, std::array<int, size>{I + 1}));
					else
						return convert2<I>(push_back(arr, std::array<int, 4 - (N % 4)>{}));
				}
			}

			static constexpr auto convert1()
			{
				auto arr = convert2<0>(std::array<int, 0>{});
				if constexpr (arr.size() % 4 > 0)
					return push_back(arr, std::array<int, 4 - (arr.size() % 4)>{});
				else
					return arr;
			}

			static constexpr auto convert0()
			{
				constexpr auto arr = convert1();

				auto f = [&]<size_t...I>(std::index_sequence<I...>)constexpr {
					return std::array<std::ptrdiff_t, sizeof...(I)>{(std::find(arr.begin(), arr.end(), I + 1) - arr.begin())...};
				};

				return f(std::make_index_sequence<std::tuple_size_v<T>>{});
			}
			
			static auto convert(const T& data)
			{

				constexpr auto indices = convert0();
				constexpr auto places = convert1();
				std::array<float, places.size()> arr = {};
					
				auto f = [&]<size_t...I>(std::index_sequence<I...>)
				{
					auto g = [&]<typename T>(const T&val, size_t i) {
						*reinterpret_cast<T*>(&arr[i]) = val;
					};
					(g(get<I>(data), indices[I]), ...);
				};

				f(std::make_index_sequence<std::tuple_size<T>::value>{});
				
				return arr;
			}

			

			using convert_t = decltype(convert(std::declval<T&>()));
			
			convert_t data;
		};
	}

	template<typename T, memory_qualifier Q = memory_qualifier::dynamic>
	class constant_buffer : public basic_buffer<details::constant_buffer_align<T>, Q, buffer_binding::constant_buffer>
	{
	public:

		template<typename C>
		explicit constant_buffer(C& ctx)
			: basic_buffer<details::constant_buffer_align<T>, Q, buffer_binding::constant_buffer>(1, ctx)
		{}

		template<typename C>
		constant_buffer(const T& data, C& ctx)
			: basic_buffer<details::constant_buffer_align<T>, Q, buffer_binding::constant_buffer>(std::array<details::constant_buffer_align<T>, 1>{ {details::constant_buffer_align<T>{data}}}, ctx)
		{}

		void emplace(const T&data)
		{
			auto m = this->map(map_t::write_discard);
			*m.begin() = details::constant_buffer_align<T>(data);
		}

	private:

	};


}
