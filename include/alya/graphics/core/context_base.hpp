#pragma once
#include<alya/ui/core/window_base.hpp>
#include<alya/graphics/core/depth_stencil_state.hpp>
#include<optional>
#include<alya/graphics/core/details/context_base_fwd.hpp>
#include<alya/graphics/core/details/impl/d3d11_context.hpp>

namespace alya::graphics::core
{

	constexpr struct default_frame_buffer_t {} default_frame_buffer{};

	class context_base
	{
	public:

		using native_window_handle_type = details::d3d11_context::native_window_handle_type;

		context_base(native_window_handle_type window, details::pixel_type color, details::pixel_type depth, size_t samples)
			: impl_(window, color, depth, samples)
		{}

		void clear_depth(float depth)noexcept
		{
			impl_.default_frame_buffer().clear_depth(depth);
		}

		void clear_color(float r, float g, float b, float a)noexcept
		{
			impl_.default_frame_buffer().clear_color(r, g, b, a, 0);
		}

		void present(bool sync = true)
		{
			impl_.default_frame_buffer().present(sync);
		}

		void set_viewport(int x, int y, int width, int height)
		{
			impl_.set_viewport(x, y, width, height);
		}

		void draw(size_t from, size_t count)
		{
			impl_.draw(from, count);
		}

	protected:
		details::d3d11_context impl_;
		friend class buffer_base;
		friend class texture2d_base;
		friend class vertex_shader;
		friend class pixel_shader;
		friend class sampler;
		template<typename...T>
		friend class vertex_stream;
	};

}
