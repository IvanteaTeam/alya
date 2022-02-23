#pragma once
#include<alya/graphics/core/vertex_shader.hpp>
#include<alya/graphics/core/pixel_shader.hpp>
#include<alya/ui/core/window_base.hpp>
#include<alya/graphics/core/frame_buffer_base.hpp>
#include<alya/graphics/core/vertex_stream.hpp>
#include<alya/graphics/core/sampler.hpp>
#include<alya/graphics/core/depth_stencil_state.hpp>
#include<alya/graphics/core/texture_view.hpp>
#include<optional>

namespace alya::graphics::core
{

	struct default_frame_buffer_t {};
	struct vertex_shader_target_t {};
	struct pixel_shader_target_t {};


	constexpr default_frame_buffer_t default_frame_buffer{};
	constexpr vertex_shader_target_t vertex_shader_target{};
	constexpr pixel_shader_target_t pixel_shader_target{};

	class context_base
	{
	public:

		context_base(ui::core::window_base::native_handle_type, details::pixel_type color, details::pixel_type depth, size_t samples);
		context_base(context_base&&) = delete;
		context_base&operator=(context_base&&) = delete;
		context_base(const context_base&) = delete;
		context_base& operator=(const context_base&) = delete;

		void bind_frame_buffer(frame_buffer_base&fb);
		void bind_frame_buffer(default_frame_buffer_t);
		void bind_frame_buffer(std::nullopt_t);

		template<typename...T>
		void bind_vertices(const vertex_stream<T...>& vertices)
		{
			constexpr size_t size = sizeof...(T);
			std::array<uint32_t, size> offsets, strides;
			std::array<const ID3D11Buffer*, size> buffers;
			for (size_t i = 0; i < vertices.attributes_.size(); i++)
			{
				offsets[i] = vertices.attributes_[i].offset_;
				strides[i] = vertices.attributes_[i].stride_;
				buffers[i] = vertices.attributes_[i].buffer_.get();
			}
			bind_vertices(vertices, buffers.data(), offsets.data(), strides.data(), vertices.attributes_.size());
		}
		void bind_vertices(std::nullopt_t);

		template<typename T, memory_qualifier Q, buffer_binding B>
		void bind_constants(vertex_shader_target_t, const basic_buffer<T, Q, B>& buffer, size_t slot) requires(bitmask(B).contains(buffer_binding::constant_buffer))
		{
			bind_constants(vertex_shader_target, static_cast<const buffer_base&>(buffer), slot);
		}
		void bind_constants(vertex_shader_target_t, std::nullopt_t, size_t slot);

		template<typename T, memory_qualifier Q, buffer_binding B>
		void bind_constants(pixel_shader_target_t, const basic_buffer<T, Q, B>& buffer, size_t slot) requires(bitmask(B).contains(buffer_binding::constant_buffer))
		{
			bind_constants(pixel_shader_target, static_cast<const buffer_base&>(buffer), slot);
		}
		void bind_constants(pixel_shader_target_t, std::nullopt_t, size_t slot);

		void bind_sampler(vertex_shader_target_t, const sampler&, size_t slot);
		void bind_sampler(vertex_shader_target_t, std::nullopt_t, size_t slot);
		void bind_sampler(pixel_shader_target_t, const sampler&, size_t slot);
		void bind_sampler(pixel_shader_target_t, std::nullopt_t, size_t slot);

		void clear_color(float, float, float, float);
		void clear_depth(float);
		void clear_stencil(int);
		void clear_depth_stencil(float, int);

		void bind_vertex_shader(const vertex_shader&);
		void bind_vertex_shader(std::nullopt_t);
		void bind_pixel_shader(const pixel_shader&);
		void bind_pixel_shader(std::nullopt_t);

		void present(bool sync = true);

		void set_viewport(int, int, int, int);

		void draw(size_t from, size_t count);

		void set_depth_stencil_state(const depth_stencil_state&);

		void bind_texture(vertex_shader_target_t, const texture_view& view, size_t slot);
		void bind_texture(vertex_shader_target_t, std::nullopt_t, size_t slot);
		void bind_texture(pixel_shader_target_t, const texture_view&view, size_t slot);
		void bind_texture(pixel_shader_target_t, std::nullopt_t, size_t slot);

		void clear();

	private:

		void bind_vertices(const vertex_stream_base&, const ID3D11Buffer*const* buffers, const uint32_t*offsets, const uint32_t*strides, size_t count);

		void bind_constants(vertex_shader_target_t, const buffer_base&buffer, size_t slot);
		void bind_constants(pixel_shader_target_t, const buffer_base&buffer, size_t slot);

		void resize_buffers(size_t w, size_t h);

		mutable d3d11::device_ptr device;
		mutable d3d11::device_context_ptr device_context;
		windows::com::shared_ptr<IDXGIFactory> factory;
		windows::com::shared_ptr<IDXGISwapChain> swap_chain;
		d3d11::rtv_ptr back_buffer, back_buffer_ms;
		d3d11::dsv_ptr depth_buffer;
		//std::optional<basic_texture<render_buffer::pixel_format>> back_buffer_texture;
		//std::optional<render_buffer> back_buffer_;
		//std::optional<graphics::render_buffer_base> back_buffer_;
		//std::optional<graphics::details::texture2d_base> back_buffer_texture;

		ui::core::window_base::native_handle_type hwnd;
		size_t width, height;
		size_t samples;
		details::pixel_type color, depth;

		friend class frame_buffer_base;
		friend class d3d11::object_base;
		friend class texture2d_base;
		friend class buffer_base;
		friend class vertex_stream_base;
	};

}
