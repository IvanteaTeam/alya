#pragma once
#include<Windows.h>
#include<alya/utility/windows/win32_error.hpp>

namespace alya::ui::core::details
{

	class window_class
	{
	public:
		
		window_class(const wchar_t* name, HINSTANCE instance, WNDPROC proc) : 
			name_(name),
			instance_(instance)
		{
			WNDCLASSEXW wc{};
			wc.cbSize = sizeof(wc);
			wc.lpfnWndProc = proc;
			wc.hInstance = instance;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
			wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = name_;
			wc.hbrBackground = nullptr;
			wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wc.style = 0;
			auto res = RegisterClassExW(&wc);
			if (!res)
				throw std::system_error{windows::make_error_code(GetLastError())};
		}
		~window_class()
		{
			UnregisterClassW(name_, instance_);
		}

		const wchar_t* name()const noexcept
		{
			return name_;
		}
		HINSTANCE instance()const noexcept
		{
			return instance_;
		}

	private:
		const wchar_t* name_;
		HINSTANCE instance_;
	};

}
