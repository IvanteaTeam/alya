#include<alya/graphics/core/frame_buffer_base.hpp>
#include<alya/graphics/core/details/debug.hpp>

namespace alya::graphics::core
{

	frame_buffer_base::frame_buffer_base(d3d11::device_ptr device, d3d11::device_context_ptr ctx, frame_buffer_base**curr)
		: object_base(device, ctx), curr_frame_buffer(curr)
	{}

	void frame_buffer_base::attach_color_buffer(color_buffer_view_base*view, size_t slot)
	{
		rtvs[slot] = view ? view->rtv : nullptr;
		if (*curr_frame_buffer == this)
			bind();
	}

	void frame_buffer_base::attach_depth_stencil_buffer(depth_stencil_buffer_view_base* view)
	{
		dsv = view ? view->dsv : nullptr;
		if (*curr_frame_buffer == this)
			bind();
	}

	void frame_buffer_base::bind()
	{
		std::array<d3d11::rtv_t*, 8> ptrs;
		std::transform(rtvs.begin(), rtvs.end(), ptrs.begin(), [](auto& ptr) { return ptr.get(); });
		ALYA_GFX_CALL(get_device_context()->OMSetRenderTargets(ptrs.size(), ptrs.data(), dsv.get()));
		*curr_frame_buffer = this;
	}

	void frame_buffer_base::clear()
	{
		rtvs = {};
		dsv = nullptr;
	}
	
}
