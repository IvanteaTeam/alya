#pragma once
#include<alya/graphics/texture2d_base.hpp>
#include<alya/graphics/color_buffer_view_base.hpp>
#include<alya/graphics/depth_stencil_buffer_view_base.hpp>

namespace alya::graphics
{

	class frame_buffer_base : public d3d11::object_base
	{
	protected:

		template<typename C>
		frame_buffer_base(C& ctx) :
			frame_buffer_base(get_device(ctx), get_device_context(ctx), &ctx.current_frame_buffer)
		{}

		frame_buffer_base(frame_buffer_base&& other) : object_base(std::move(other)), curr_frame_buffer(other.curr_frame_buffer), rtvs(other.rtvs), dsv(other.dsv)
		{
			if (&other == *curr_frame_buffer)
				*curr_frame_buffer = this;
		}

		frame_buffer_base& operator=(frame_buffer_base&& other)
		{
			curr_frame_buffer = other.curr_frame_buffer;
			rtvs = std::move(other.rtvs);
			dsv = std::move(other.dsv);
			if (&other == *curr_frame_buffer)
				*curr_frame_buffer = this;
			return *this;
		}

		void attach_color_buffer(color_buffer_view_base*, size_t slot);
		void attach_depth_stencil_buffer(depth_stencil_buffer_view_base*);
		void clear();

	private:

		void bind();

		frame_buffer_base(d3d11::device_ptr, d3d11::device_context_ptr, frame_buffer_base**);
		frame_buffer_base** curr_frame_buffer;
		std::array<d3d11::rtv_ptr, 8>rtvs;
		d3d11::dsv_ptr dsv;
		friend class context_base;
	};

}
