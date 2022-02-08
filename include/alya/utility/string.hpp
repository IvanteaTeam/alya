#pragma once
#include<string>
#include<utf8.h>

namespace alya
{
    using char_t = char8_t;
    using string = std::basic_string<char_t>;
    using string_view = std::basic_string_view<char_t>;
}
