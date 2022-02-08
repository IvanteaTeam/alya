#pragma once
#include<type_traits>
#include<limits>
#include<concepts>
#include<algorithm>

namespace alya
{

	template<std::integral T, std::floating_point F = float>
	class normalized
	{
	public:

		using value_type = T;
		using floating_point_type = F;

		constexpr normalized() noexcept : normalized(0) {}
		constexpr normalized(const value_type&value) noexcept : value_(value) {}
		constexpr normalized(std::floating_point auto const& value) noexcept : 
			normalized(
				std::clamp(
					floating_point_type(value), 
					floating_point_type(-1), 
					floating_point_type(1)
				) * floating_point_type((std::numeric_limits<value_type>::max)())
			)
		{}

		constexpr value_type value()const noexcept
		{
			return value_;
		}

		constexpr floating_point_type count()const noexcept
		{
			return floating_point_type(value()) / floating_point_type((std::numeric_limits<value_type>::max)());
		}

		friend constexpr normalized operator+(const normalized&a, const normalized&b)noexcept
		{
			return a.value() + b.value();
		}

		constexpr normalized& operator+=(const normalized&b)noexcept
		{
			return *this = *this + b;
		}

		friend constexpr normalized operator-(const normalized& a, const normalized& b)noexcept
		{
			return a.value() - b.value();
		}

		constexpr normalized& operator-=(const normalized& b)noexcept
		{
			return *this = *this - b;
		}

		friend constexpr normalized operator*(const normalized&a, floating_point_type factor)noexcept
		{
			return a.value() * factor;
		}

		constexpr normalized& operator*=(floating_point_type factor)noexcept
		{
			return *this = *this * factor;
		}

		friend constexpr normalized operator/(const normalized&a, floating_point_type factor)noexcept
		{
			return a.value() / factor;
		}

		constexpr normalized& operator/=(floating_point_type factor)noexcept
		{
			return *this = *this / factor;
		}

	private:
		value_type value_;
	};

	template<typename T>
	struct is_normalized : std::false_type {};

	template<typename T, typename U>
	struct is_normalized<normalized<T, U>> : std::true_type {};

	template<typename T>
	struct is_signed : std::is_signed<T> {};
	
	template<typename T, typename U>
	struct is_signed<normalized<T, U>> : is_signed<typename normalized<T, U>::value_type> {};

	using uint8n_t = normalized<uint8_t>;
	using int8n_t = normalized<int8_t>;
	using uint16n_t = normalized<uint16_t>;
	using int16n_t = normalized<int16_t>;
	using int32n_t = normalized<int32_t>;

}
