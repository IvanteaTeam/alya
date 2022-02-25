#pragma once
#include<alya/graphics/core/details/impl/d3d11_frame_buffer.hpp>
#include<alya/graphics/core/details/impl/dxgi_swap_chain.hpp>
#include<alya/graphics/core/details/impl/d3d11_texture2d.hpp>
#include<optional>

struct ID3D11Device;

namespace alya::graphics::core::details
{

	class d3d11_context;

	class d3d11_default_frame_buffer : public d3d11_frame_buffer
	{
	public:

		using native_window_handle_type = dxgi_swap_chain::native_window_handle_type;

		d3d11_default_frame_buffer(native_window_handle_type window, pixel_type back_buffer_pixel, pixel_type depth_buffer_pixel, size_t samples, windows::com::shared_ptr<ID3D11Device>);

		void present(bool sync)
		{
			do_present(sync);
			size_t width_ = window_width(), height_ = window_height();
			if (!((std::max)(width_, dxgi_swap_chain::min_width) == width() && (std::max)(height_, dxgi_swap_chain::min_height) == height()))
			{
				resize_buffers(width_, height_);
			}
		}

		size_t width()const noexcept
		{
			return swap_chain_.width();
		}

		size_t height()const noexcept
		{
			return swap_chain_.height();
		}

		size_t samples()const noexcept
		{
			return samples_;
		}

		native_window_handle_type window()const noexcept
		{
			return swap_chain_.window();
		}

	private:

		void do_present(bool sync)noexcept;
		void resize_buffers(size_t, size_t);

		size_t window_width()const noexcept;
		size_t window_height()const noexcept;

		dxgi_swap_chain swap_chain_;
		pixel_type back_buffer_pixel_, depth_buffer_pixel_;
		size_t samples_;
		std::optional<d3d11_texture2d> back_buffer_ms_;
	};

}
