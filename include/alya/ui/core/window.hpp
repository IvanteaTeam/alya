#pragma once
#include<alya/ui/core/basic_window.hpp>
#include<alya/async/inline_executor.hpp>

namespace alya::ui::core
{
	
	using window = basic_window<async::inline_executor>;

}
