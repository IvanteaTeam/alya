#pragma once
#include<alya/ui/core/event_base.hpp>
#include<alya/ui/core/key.hpp>

namespace alya::ui::core
{

	class focus_event : public event_base  {};

	class should_close : public event_base {};
	class should_minimize : public event_base {};
	class should_maximize : public event_base {};
	class should_restore : public event_base {};
	class resized : public event_base {};

	class key_event : public event_base 
	{
	public:
		explicit key_event(core::key key) noexcept : key(key) {}
		core::key key;
	};

	class key_down : public key_event 
	{
		using key_event::key_event;
	};

	class key_press : public key_event
	{
		using key_event::key_event;
	};

	class key_up : public key_event
	{
		using key_event::key_event;
	};

	class text_input : public event_base 
	{
	public:
		explicit text_input(char32_t symbol) noexcept : symbol(symbol) {}
		char32_t symbol;
	};

	class mouse_event : public event_base
	{
	public:
		mouse_event(int x, int y) noexcept : x(x), y(y) {}
		int x, y;
	};

	class mouse_move : public mouse_event
	{
		using mouse_event::mouse_event;
	};

	class mouse_down : public mouse_event
	{
		using mouse_event::mouse_event;
	};

	class left_button_down : public mouse_down
	{
		using mouse_down::mouse_down;
	};

	class right_button_down : public mouse_down
	{
		using mouse_down::mouse_down;
	};

	class mouse_up : public mouse_event
	{
		using mouse_event::mouse_event;
	};

	class left_button_up : public mouse_up
	{
		using mouse_up::mouse_up;
	};

	class right_button_up : public mouse_up
	{
		using mouse_up::mouse_up;
	};

	class raw_mouse_input : public mouse_event
	{
		using mouse_event::mouse_event;
	};

}
