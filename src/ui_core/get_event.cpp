#include<alya/ui/core/get_event.hpp>
#include<Windows.h>

namespace alya::ui::core
{

	bool get_event()noexcept
	{
		MSG msg;
		if (GetMessageW(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			return true;
		}
		return false;
	}

	bool poll_event()noexcept
	{
		MSG msg;
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			return true;
		}
		return false;
	}

}
