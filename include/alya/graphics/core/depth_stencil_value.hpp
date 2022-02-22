#pragma once
#include<alya/utility/pixel_traits.hpp>

namespace alya::graphics::core
{
	template<size_t DepthBits, size_t StencilBits>
	class depth_stencil_value
	{
	public:
		static const size_t depth_bits = DepthBits;
		static const size_t stencil_bits = StencilBits;
	private:
		static_assert(DepthBits % 8 == 0 && StencilBits % 8 == 0 );

		

		std::aligned_storage_t<(DepthBits + StencilBits) / 8> _;
	};

	template<typename T>
	struct is_depth_stencil_value : std::false_type {};

	template<size_t D, size_t S>
	struct is_depth_stencil_value<depth_stencil_value<D, S>> : std::true_type {};

}

namespace alya
{

	template<size_t D, size_t S>
	struct pixel_traits<graphics::core::depth_stencil_value<D, S>>
	{
	public:
		using value_type = graphics::core::depth_stencil_value<D, S>;
		static constexpr auto depth_bits = value_type::depth_bits;
		static constexpr auto stencil_bits = value_type::stencil_bits;
	};

}
