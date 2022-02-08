#pragma once
#include<string>
#include<cassert>
#include<sstream>
#include<alya/utility/string.hpp>

#include<iostream>


#define _ALYA_MAKE_DEBUG_MESSAGE(msg) (([&](){ ::std::stringstream s; s << msg; return s.str();})())

#ifdef NDEBUG
#define _ALYA_DEBUG_REPORT(msg) { std::cerr << _ALYA_MAKE_DEBUG_MESSAGE(msg); std::terminate(); }
#else
#define _ALYA_DEBUG_REPORT(msg) ::_wassert( [&](auto str){ std::wstring out; utf8::utf8to16(str.begin(), str.end(), std::back_inserter(out)); return out;}(_ALYA_MAKE_DEBUG_MESSAGE(msg)).c_str(), _CRT_WIDE(__FILE__), __LINE__)
#endif

#ifdef NDEBUG
#define ALYA_ASSERT(expr, msg)
#else
#define ALYA_ASSERT(expr, msg) if(!(expr)) _ALYA_DEBUG_REPORT(msg); else {};
#endif
