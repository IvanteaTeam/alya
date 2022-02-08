#pragma once
#include<alya/utility/image.hpp>

namespace alya
{

	template<typename T>
	class basic_image2d_view
	{
	public:

		using value_type = T;

		constexpr basic_image2d_view() noexcept : basic_image2d_view(0, 0, nullptr) {}

		constexpr basic_image2d_view(const basic_image2d_view& other) = default;
		constexpr basic_image2d_view(basic_image2d_view&&) = default;

		constexpr basic_image2d_view(compatible_image2d<basic_image2d_view> auto const&image) noexcept
			: basic_image2d_view(image.width(), image.height(), pixel_cast<value_type const*>(image.data()), image.row_alignment())
		{}

		constexpr basic_image2d_view(size_t width, size_t height, const typename value_type* data, size_t row_alignment = 0) noexcept
			: width_(width), height_(height), data_(data), row_alignment_(row_alignment == 0 ? width * sizeof(value_type) : row_alignment)
		{}

	 	constexpr const typename value_type* data()const noexcept
		{
			return data_;
		}
			
		constexpr size_t width()const noexcept
		{
			return width_;
		}

		constexpr size_t height()const noexcept
		{
			return height_;
		}

		constexpr size_t row_alignment()const noexcept
		{
			return row_alignment_;
		}

	private:
		size_t height_;
		size_t width_;
		size_t row_alignment_;
		const typename value_type * data_;
	};

	template<typename T>
	class basic_image1d_view
	{
	public:

		using value_type = T;

		static_assert(is_pixel_format<pixel_format>::value);

		constexpr basic_image1d_view() noexcept : basic_image1d_view(0, 0, nullptr) {}

		constexpr basic_image1d_view(const basic_image1d_view&) = default;
		constexpr basic_image1d_view(basic_image1d_view&&) = default;

		constexpr basic_image1d_view(compatible_image1d<basic_image1d_view> auto const& image) noexcept
			: basic_image1d_view(image.width(), image.data())
		{}

		constexpr basic_image1d_view(size_t width, const typename value_type* data) noexcept
			: width_(width), data_(data)
		{}

		constexpr const typename value_type* data()const noexcept
		{
			return data_;
		}

		constexpr size_t width()const noexcept
		{
			return width_;
		}

	private:
		size_t width_;
		const typename value_type * data_;
	};

}
