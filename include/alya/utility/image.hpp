#pragma once
#include<alya/utility/pixel_traits.hpp>

namespace alya
{

	template<typename T>
	concept image3d = requires(T i)
	{
		{i.width()}->std::convertible_to<size_t>;
		{i.height()}->std::convertible_to<size_t>;
		{i.depth()}->std::convertible_to<size_t>;
		{i.row_alignment()}->std::convertible_to<size_t>;
		{i.slice_alignment()}->std::convertible_to<size_t>;
		{i.data()}->std::convertible_to<typename T::value_type const*>;
	};

	template<typename T>
	concept image2d = !image3d<T> && requires(T i)
	{
		{i.width()}->std::convertible_to<size_t>;
		{i.height()}->std::convertible_to<size_t>;
		{i.row_alignment()}->std::convertible_to<size_t>;
		{i.data()}->std::convertible_to<typename T::value_type const*>;
	};

	template<typename T>
	concept image1d = !image2d<T> && !image3d<T> && requires(T i)
	{
		{i.width()}->std::convertible_to<size_t>;
		{i.data()}->std::convertible_to<typename T::value_type const*>;
	};


	template<typename T, typename Other>
	concept compatible_image2d = image2d<T> && image2d<Other> && compatible_pixel<typename T::value_type, typename Other::value_type>;

	template<typename T, typename Other>
	concept compatible_image1d = image1d<T> && image1d<Other> && compatible_pixel<typename T::value_type, typename Other::value_type>;

}
