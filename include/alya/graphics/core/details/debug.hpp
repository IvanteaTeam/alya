#pragma once
#include<alya/graphics/core/details/impl/dxgi_debug_queue.hpp>
#include<alya/utility/deffer.hpp>
#include<alya/utility/assert.hpp>

#ifndef ALYA_GFX_DEBUG
#ifdef NDEBUG
#define ALYA_GFX_DEBUG 0
#else
#define ALYA_GFX_DEBUG 1
#endif
#endif



#if ALYA_GFX_DEBUG
#define ALYA_GFX_CALL(expr) (([&](const auto&){ return expr; })(alya::deffer{[]()\
{ \
	auto&queue = alya::graphics::core::details::dxgi_debug_queue();\
	if(queue.size() == 0)\
		return;\
	std::string msgs;\
	for (size_t i = 0; i < queue.size(); i++) \
		msgs += queue.get(i) + "\n";\
	_ALYA_DEBUG_REPORT(msgs);\
}\
}))
#else
#define ALYA_GFX_CALL(expr) expr
#endif

