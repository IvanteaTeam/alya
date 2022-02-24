#pragma once
#include<alya/graphics/core/details/impl/d3d11_default_frame_buffer.hpp>
#include<alya/graphics/core/details/impl/d3d11_device.hpp>
#include<alya/graphics/core/details/impl/d3d11_vertex_stream.hpp>
#include<alya/graphics/core/details/impl/d3d11_buffer.hpp>
#include<alya/graphics/core/shader_tags.hpp>
#include<alya/graphics/core/details/impl/d3d11_vertex_shader.hpp>
#include<alya/graphics/core/details/impl/d3d11_pixel_shader.hpp>
#include<alya/graphics/core/details/impl/d3d11_sampler.hpp>
#include<alya/graphics/core/details/impl/d3d11_shader_resource_view.hpp>

struct ID3D11DeviceContext;

namespace alya::graphics::core::details
{

	class d3d11_context
	{
	public:

		using native_window_handle_type = d3d11_default_frame_buffer::native_window_handle_type;
		using native_handle_type = windows::com::shared_ptr<ID3D11DeviceContext>;

		d3d11_context(native_window_handle_type window, pixel_type back_buffer_pixel, pixel_type depth_buffer_pixel, size_t samples);
	
		native_handle_type native_handle()const noexcept
		{
			return context_;
		}

		d3d11_device& device()noexcept
		{
			return device_;
		}

		d3d11_default_frame_buffer& default_frame_buffer()noexcept
		{
			return default_frame_buffer_;
		}

		void set_viewport(int, int, int, int);
		void draw(size_t, size_t);

		void bind_frame_buffer(d3d11_frame_buffer&);

		template<size_t Size>
		void bind_vertices(const d3d11_vertex_stream<Size>&vertices)
		{
			bind_input_layout(vertices.input_layout_.native_handle().get());
			do_bind_vertices(reinterpret_cast<const ID3D11Buffer*const*>(vertices.buffers_.data()), vertices.strides_.data(), vertices.offsets_.data(), vertices.buffers_.size());
		}

		void bind_constants(vertex_shader_tag_t, const d3d11_buffer&, size_t);
		void bind_constants(pixel_shader_tag_t, const d3d11_buffer&, size_t);

		void bind_texture(pixel_shader_tag_t, const d3d11_shader_resource_view&, size_t);

		void bind_vertex_shader(const d3d11_vertex_shader&);
		void bind_pixel_shader(const d3d11_pixel_shader&);

		void bind_sampler(pixel_shader_tag_t, const d3d11_sampler&, size_t);

	private:

		void do_bind_vertices(const ID3D11Buffer* const*, const uint32_t* strides, const uint32_t* offsets, size_t count);
		void bind_input_layout(const ID3D11InputLayout*);

		d3d11_device device_;
		d3d11_default_frame_buffer default_frame_buffer_;
		windows::com::shared_ptr<ID3D11DeviceContext> context_;
	};

}
