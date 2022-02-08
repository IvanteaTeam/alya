#pragma once
#include<cstdint>
#include<string>

namespace alya::ui::core
{

	class key
	{
	public:

		using code_t = uint32_t;

		enum class virtual_code : code_t
		{
			_0 = '0',
			_1, _2, _3, _4, _5, _6, _7, _8, _9,

			A = 'A',
			B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

			F0 = 0x70,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

			num_0 = 0x60,
			num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9,

			escape = 0x1B,
			del = 0x2E,
			home = 0x24,

			enter = 0x0D,
			space = 0x20,
			tab = 0x09,
			backspace = 0x08,


			left_alt = 0xA4,
			right_alt = 0xA5,

			left_control = 0xA2,
			right_control = 0xA3,

			left_shift = 0xA0,
			right_shift = 0xA1,

			left_mouse_button = 0x01,
			right_mouse_button = 0x02,

			left_windows = 0x5B,
			right_windows = 0x5C,

			num_lock = 0x90,
			scroll_lock = 0x91,
			caps_lock = 0x14,

			minus = 0xBD,
			equals = 0xBB,

			open_brackets = 0xDB,
			close_brackets = 0xDD,
			back_slash = 0xDC,

			semicolon = 0xBA,
			quote = 0xDE,

			comma = 0xBC,
			period = 0xBE,
			slash = 0xBF,

			apostrophe = 0xC0,

			arrow_up = 0x26,
			arrow_right = 0x27,
			arrow_down = 0x28,
			arrow_left = 0x25,

			multiply = 0x6A,
			substract = 0x6D,
			add = 0x6B,
			divide = 0x6F,
			decimal = 0x6E
		};

		using enum virtual_code;

		constexpr key()noexcept : key(0) {}
		constexpr key(virtual_code c)noexcept : key(static_cast<code_t>(c)) {}
		constexpr key(code_t c)noexcept : code_(c) {}

		constexpr code_t code()const noexcept
		{
			return code_;
		}

		std::string_view name()const;
		bool state()const noexcept;

		friend constexpr bool operator==(const key&a, const key&b)noexcept
		{
			return a.code() == b.code();
		}

	private:
		code_t code_;
	};

}
