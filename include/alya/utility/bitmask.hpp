#pragma once
#include<type_traits>

namespace alya
{

	template<typename T>
	struct is_bitmask_enum : std::false_type{};

	template<typename T>
	class bitmask
	{
	public:

		using value_type = T;
		using underlying_type = std::underlying_type_t<value_type>;

		static_assert(is_bitmask_enum<value_type>::value, "");

		constexpr bitmask(value_type v = static_cast<value_type>(0)) noexcept : val(v) {}
		constexpr bitmask(const bitmask&mask)noexcept : val(mask.value()) {}

		constexpr value_type value()const noexcept
		{
			return val;
		}

		constexpr void emplace(const bitmask& mask)noexcept
		{
			val = static_cast<value_type>(static_cast<underlying_type>(val) | static_cast<underlying_type>(mask.value()));
		}

		constexpr void erase(const bitmask& mask)noexcept
		{
			val = static_cast<value_type>(static_cast<underlying_type>(val) & ~static_cast<underlying_type>(mask.value()));
		}

		constexpr bool contains(value_type v)const noexcept
		{
			return (static_cast<underlying_type>(val) | static_cast<underlying_type>(v)) == static_cast<underlying_type>(val);
		}

		constexpr operator value_type()const noexcept
		{
			return value();
		}

	private:
		value_type val;
	};

	template<typename T>
	constexpr bitmask<T> operator|(bitmask<T> a, T b)
	{
		a.emplace(b);
		return a;
	}

	template<typename T>
	constexpr bitmask<T> operator|(T a, bitmask<T> b)
	{
		return b | a;
	}

}

template<typename T, typename _ = ::std::enable_if_t<alya::is_bitmask_enum<T>::value>>
constexpr alya::bitmask<T> operator|(T a, T b)
{
	return alya::bitmask<T>(a) | b;
}
