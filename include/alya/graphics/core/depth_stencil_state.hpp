#pragma once
#include<d3d11.h>
#include<numeric>

namespace alya::graphics::core
{

	enum class stencil_operation
	{
		keep = D3D11_STENCIL_OP_KEEP,
		zero = D3D11_STENCIL_OP_ZERO,
		replace = D3D11_STENCIL_OP_REPLACE,
		invert = D3D11_STENCIL_OP_INVERT,
		increment = D3D11_STENCIL_OP_INCR_SAT,
		increment_wrap = D3D11_STENCIL_OP_INCR,
		decrement = D3D11_STENCIL_OP_DECR_SAT,
		decrement_wrap = D3D11_STENCIL_OP_DECR,
	};

	enum class comparison_function
	{
		less = D3D11_COMPARISON_LESS,
		less_equal = D3D11_COMPARISON_LESS_EQUAL,
		greater = D3D11_COMPARISON_GREATER,
		greater_equal = D3D11_COMPARISON_GREATER_EQUAL,
		equal = D3D11_COMPARISON_EQUAL,
		not_equal = D3D11_COMPARISON_NOT_EQUAL,
		never = D3D11_COMPARISON_NEVER,
		always = D3D11_COMPARISON_ALWAYS
	};

	struct depth_stencil_state
	{

		bool depth_test_enable = true;
		bool rewrite_depth = true;
		bool stencil_enable = false;
		comparison_function depth_cmp, stencil_cmp;
		uint8_t stencil_read_mask, stencil_write_mask;
		stencil_operation stencil_fail, stencil_pass_depth_fail, stencil_pass_depth_pass;
		uint32_t stencil_reference;
	};

}
