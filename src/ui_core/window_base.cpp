#include<alya/ui/core/window_base.hpp>
#include<alya/ui/core/details/window_class.hpp>
#include<alya/ui/core/events.hpp>
#include<hidusage.h>
#include<vector>

namespace alya::ui::core
{

	window_base::window_base(int x, int y, int width, int height) : 
		handle_(nullptr, [](native_handle_type handle) { if (handle)DestroyWindow(handle); }),
		is_open_(false),
		has_focus_(false)
	{
		handle_.reset(CreateWindowExW(0, get_window_class().name(), L"", WS_OVERLAPPEDWINDOW, x, y, width, height, nullptr, nullptr, get_window_class().instance(), this));
		if (!handle_)
			throw std::system_error{ windows::make_error_code(GetLastError()) };
		register_raw_input_device();
	}

	void window_base::register_raw_input_device()
	{
		RAWINPUTDEVICE rid;
		rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
		rid.usUsage = HID_USAGE_GENERIC_MOUSE;
		rid.dwFlags = 0;
		rid.hwndTarget = handle_.value();

		if (!RegisterRawInputDevices(&rid, 1, sizeof(rid)))
			throw std::system_error{windows::make_error_code(GetLastError())};

	}

	const details::window_class& window_base::get_window_class()
	{
		static details::window_class wc(L"AlyaWindowClass", GetModuleHandleW(nullptr), &window_procedure);
		return wc;
	}

	void window_base::show(int s)noexcept
	{
		ShowWindow(handle_.value(), s);
	}

	void window_base::open()noexcept
	{
		show(SW_SHOW);
		is_open_ = true;
	}

	void window_base::close()noexcept
	{
		show(SW_HIDE);
		is_open_ = false;
	}

	void window_base::restore()noexcept
	{
		show(SW_RESTORE);
	}

	void window_base::minimize()noexcept
	{
		show(SW_MINIMIZE);
	}

	void window_base::maximize()noexcept
	{
		show(SW_MAXIMIZE);
	}

	bool window_base::has_focus()const noexcept
	{
		return has_focus_;
	}

	bool window_base::is_open()const noexcept
	{
		return is_open_;
	}

	window_base::native_handle_type window_base::native_handle()noexcept
	{
		return handle_.value();
	}

	int window_base::width()const noexcept
	{
		RECT rect;
		GetWindowRect(handle_.value(), &rect);
		return rect.right - rect.left;
	}

	int window_base::height()const noexcept
	{
		RECT rect;
		GetWindowRect(handle_.value(), &rect);
		return rect.bottom - rect.top;
	}

	int window_base::client_width()const noexcept
	{
		RECT rect;
		GetClientRect(handle_.value(), &rect);
		return rect.right - rect.left;
	}

	int window_base::client_height()const noexcept
	{
		RECT rect;
		GetClientRect(handle_.value(), &rect);
		return rect.bottom - rect.top;
	}

	int64_t __stdcall window_base::window_procedure(native_handle_type handle, uint32_t msg, uint64_t wparam, int64_t lparam)
	{

		auto user_data = GetWindowLongPtrW(handle, GWLP_USERDATA);

		auto window = reinterpret_cast<window_base*>(user_data);

		if (window)
			return window->handle_message(msg, wparam, lparam);

		switch (msg)
		{
		case WM_CREATE:
		{
			auto create_struct = reinterpret_cast<LPCREATESTRUCTW>(lparam);
			auto window = static_cast<window_base*>(create_struct->lpCreateParams);
			
			SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<int64_t>(window));

			return window->handle_message(msg, wparam, lparam);
		}
		default:
			return DefWindowProcW(handle, msg, wparam, lparam);
		}
	}

	int64_t window_base::handle_message(uint32_t msg, uint64_t wparam, int64_t lparam)noexcept
	{

		static auto key_code_from_params = [](uint64_t wparam, int64_t lparam)->uint64_t
		{
			bool _ex = (1 << 24) & lparam;

			if (wparam == VK_MENU && _ex)
				return VK_RMENU;
			if (wparam == VK_CONTROL && _ex)
				return VK_RCONTROL;
			if (wparam == VK_SHIFT && GetAsyncKeyState(VK_RSHIFT))
				return VK_RSHIFT;
			if (wparam == 91 && GetAsyncKeyState(VK_RWIN))
				return VK_RWIN;
			return wparam;
		};

		switch (msg)
		{
		case WM_SETFOCUS:
			has_focus_ = true;
			handle_event(std::make_unique<focus_event>());
			return 0;
		case WM_KILLFOCUS:
			has_focus_ = false;
			handle_event(std::make_unique<focus_event>());
			return 0;
		case WM_CREATE:
			return 0;
		case WM_SYSCOMMAND:
		{
			auto wp = wparam & 0xfff0;
			switch (wp)
			{
			case SC_CLOSE:
				handle_event(std::make_unique<should_close>());
				return 0;
			case SC_MAXIMIZE:
				handle_event(std::make_unique<should_maximize>());
				return 0;
			case SC_MINIMIZE:
				handle_event(std::make_unique<should_minimize>());
				return 0;
			case SC_RESTORE:
				handle_event(std::make_unique<should_restore>());
				return 0;
			default:
				return DefWindowProcW(handle_.value(), msg, wparam, lparam);
			}
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if (lparam & 0x40000000)
				handle_event(std::make_unique<key_press>(static_cast<key::code_t>(key_code_from_params(wparam, lparam))));
			else
				handle_event(std::make_unique<key_down>(static_cast<key::code_t>(key_code_from_params(wparam, lparam))));
			return 0;
		case WM_KEYUP:
			handle_event(std::make_unique<key_up>(static_cast<key::code_t>(key_code_from_params(wparam, lparam))));
			return 0;
		case WM_CHAR:
			handle_event(std::make_unique<text_input>(static_cast<char32_t>(wparam)));
			return 0;
		case WM_SIZE:
			handle_event(std::make_unique<resized>());
			return 0;
		case WM_MOUSEMOVE:
			handle_event(std::make_unique<mouse_move>(LOWORD(lparam), HIWORD(lparam)));
			return 0;
		case WM_LBUTTONDOWN:
			handle_event(std::make_unique<left_button_down>(LOWORD(lparam), HIWORD(lparam)));
			return 0;
		case WM_LBUTTONUP:
			handle_event(std::make_unique<left_button_up>(LOWORD(lparam), HIWORD(lparam)));
			return 0;
		case WM_RBUTTONDOWN:
			handle_event(std::make_unique<right_button_down>(LOWORD(lparam), HIWORD(lparam)));
			return 0;
		case WM_RBUTTONUP:
			handle_event(std::make_unique<right_button_up>(LOWORD(lparam), HIWORD(lparam)));
			return 0;
		case WM_INPUT:
		{
			uint32_t size = 0;
			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

			std::vector<char> rid(size);

			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, rid.data(), &size, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)rid.data();

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
				{
					auto x = raw->data.mouse.lLastX;
					auto y = raw->data.mouse.lLastY;
					handle_event(std::make_unique<raw_mouse_input>(x, y));
				}
			}
			return 0;
		}
		default:
			return DefWindowProcW(handle_.value(), msg, wparam, lparam);
		}
	}

}
