#include<alya/utility/windows/win32_error.hpp>
#include<Windows.h>
#include<alya/utility/string.hpp>

namespace alya::windows
{

	class win32_category : public std::error_category
	{
	public:
		virtual const char* name()const noexcept override
		{
			return "winapi error";
		}
		virtual std::string message(int e)const noexcept override
		{
			std::wstring str;
			str.resize(1000);
			DWORD res = FormatMessageW(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, static_cast<DWORD>(e), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), str.data(), str.length(), nullptr);

			if (res)
			{
				try {
					std::string out;
					utf8::utf16to8(str.begin(), str.begin() + res, std::back_inserter(out));
					return out;
				}
				catch (const std::exception&)
				{
					
				}
			}

			return "unrecognised error [" + std::to_string(e) + "]";
		}
	private:
		win32_category() = default;
		friend const std::error_category& error_category();
	};


	const std::error_category& error_category()
	{
		static win32_category c;
		return c;
	}

}
