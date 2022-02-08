#include<alya/ui/core/key.hpp>
#include<Windows.h>
#include<map>
#include<array>

namespace alya::ui::core
{

	bool key::state()const noexcept
	{
		return GetAsyncKeyState(code()) != 0;
	}

	std::string_view key::name()const
	{

		auto vk_to_lparam = [](code_t c) -> uint32_t
		{

			bool ex = false;

			if (c == VK_NUMLOCK || (c >= VK_LEFT && c <= VK_DOWN) || c == VK_PRIOR || c == VK_NEXT || c == VK_END || c == VK_DELETE || c == VK_HOME)
				ex = true;

			switch (c)
			{
			case VK_UP:
				c = VK_NUMPAD8;
				break;
			case VK_DOWN:
				c = VK_NUMPAD2;
				break;
			case VK_LEFT:
				c = VK_NUMPAD4;
				break;
			case VK_RIGHT:
				c = VK_NUMPAD6;
				break;
			}

			int scan_code = MapVirtualKeyA(c, MAPVK_VK_TO_VSC_EX);

			if (scan_code == 0)
				scan_code = c;
			else if (scan_code & 0x0000ff00)
				ex = true;

			return  1 | ((scan_code & 0x000000ff) << 16) | (ex ? (1 << 24) : 0);
		};

		static std::map<uint32_t, std::string> names;

		auto lp = vk_to_lparam(code_);

		if (auto it = names.find(lp); it != names.end())
			return it->second;

		std::array<char, 20> buff;

		GetKeyNameTextA(lp, reinterpret_cast<char*>(buff.data()), buff.size());

		return names[lp] = buff.data();
	}

}
