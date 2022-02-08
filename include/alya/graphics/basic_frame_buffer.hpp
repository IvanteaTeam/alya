#pragma once
#include<alya/graphics/frame_buffer_base.hpp>
#include<alya/graphics/basic_color_buffer_view.hpp>
#include<alya/graphics/basic_depth_stencil_buffer_view.hpp>

namespace alya::graphics
{
	
	template<size_t S, bool L>
	class basic_frame_buffer : public frame_buffer_base
	{
	public:
		template<typename C>
		explicit basic_frame_buffer(C& ctx) : frame_buffer_base(ctx) {}

		void attach_color_buffer(basic_color_buffer_view<S, L>& view, size_t slot)
		{
			frame_buffer_base::attach_color_buffer(&view, slot);
		}

		void attach_color_buffer(std::nullptr_t, size_t slot)
		{
			frame_buffer_base::attach_color_buffer(nullptr, slot);
		}

		void attach_depth_stencil_buffer(basic_depth_stencil_buffer_view<S, L>& view)
		{
			frame_buffer_base::attach_depth_stencil_buffer(&view);
		}

		void attach_depth_stencil_buffer(std::nullptr_t)
		{
			frame_buffer_base::attach_depth_stencil_buffer(nullptr);
		}

	};

}
