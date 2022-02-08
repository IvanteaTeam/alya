#include<alya/utility/windows/com/intialize.hpp>
#include<alya/utility/windows/win32_error.hpp>
#include<combaseapi.h>

namespace alya::windows::com
{

	class initializer
	{
	public:
		initializer()
		{
			auto res = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			if (!(res == S_OK || res == S_FALSE))
				throw std::system_error{make_error_code(res)};
		}
		~initializer()
		{
			CoUninitialize();
		}
	private:

	};

	void initialize()
	{
		static initializer i;
	}


}
