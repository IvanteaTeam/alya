#pragma once
#include<alya/graphics/core/context_base.hpp>
#include<alya/graphics/core/frame_buffer_base.hpp>
#include<alya/graphics/core/vertex_stream.hpp>
#include<alya/graphics/core/vertex_shader.hpp>
#include<alya/graphics/core/pixel_shader.hpp>
#include<alya/graphics/core/sampler.hpp>
#include<alya/graphics/core/texture_view.hpp>
#include<alya/graphics/core/basic_buffer.hpp>

namespace alya::graphics::core
{

	template<typename C, typename D, size_t S>
	class basic_context : public context_base
	{
	public:
		explicit basic_context(ui::core::window_base&window) : basic_context(window.native_handle()) {}
		explicit basic_context(ui::core::window_base::native_handle_type window) : 
			context_base(
				window,
				details::make_pixel_type<C>(),
				details::make_pixel_type<D>(),
				S
			) {}

		void bind_frame_buffer(frame_buffer_base&frame_buffer)
		{
			impl_.bind_frame_buffer(frame_buffer.impl_);
		}

		void bind_frame_buffer(default_frame_buffer_t)
		{
			impl_.bind_frame_buffer(impl_.default_frame_buffer());
		}

		template<typename...T>
		void bind_vertices(const vertex_stream<T...>&vertices)
		{
			impl_.bind_vertices(vertices.impl_);
		}
		
		template<typename T, memory_qualifier Q, buffer_binding B>
		void bind_constants(vertex_shader_tag_t, const basic_buffer<T, Q, B>&buffer, size_t slot)
		{
			impl_.bind_constants(vertex_shader_tag, buffer.impl_, slot);
		}

		void bind_texture(pixel_shader_tag_t, texture_view&texture, size_t slot)
		{
			impl_.bind_texture(pixel_shader_tag, texture.impl_, slot);
		}

		void bind_vertex_shader(const vertex_shader&shader)
		{
			impl_.bind_vertex_shader(shader.impl_);
		}

		void bind_pixel_shader(const pixel_shader&shader)
		{
			impl_.bind_pixel_shader(shader.impl_);
		}

		void bind_sampler(pixel_shader_tag_t, const sampler&sampler, size_t slot)
		{
			impl_.bind_sampler(pixel_shader_tag, sampler.impl_, slot);
		}

	};

}
