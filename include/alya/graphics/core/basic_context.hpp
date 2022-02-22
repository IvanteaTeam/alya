#pragma once
#include<alya/graphics/core/context_base.hpp>

namespace alya::graphics::core
{

	template<typename C, typename D, size_t S>
	class basic_context : public context_base
	{
	public:
		explicit basic_context(ui::core::window_base&window) : basic_context(window.native_handle()) {}
		explicit basic_context(ui::core::window_base::native_handle_type hwnd) : 
			context_base(
				hwnd,
				details::make_pixel_type<C>(),
				details::make_pixel_type<D>(),
				S
			) {}
	};

	template<typename C, size_t S>
	class basic_context<C, void, S> : public context_base
	{
	public:
		explicit basic_context(ui::core::window_base& window) : basic_context(window.native_handle()) {}
		explicit basic_context(ui::core::window_base::native_handle_type hwnd) :
			context_base(
				hwnd,
				details::make_pixel_type<C>(),
				details::pixel_type::none,
				S
			) {}
	};

}
