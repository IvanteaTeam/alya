#pragma once
#include<system_error>

namespace alya::windows
{

	const std::error_category& error_category();

	inline std::error_code make_error_code(int code)
	{
		return std::error_code{ code, error_category() };
	}

}
