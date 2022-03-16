#include<alya/graphics/core/details/impl/d3d11_default_frame_buffer.hpp>
#include<alya/graphics/core/details/impl/d3d11_texture2d.hpp>
#include<alya/graphics/core/details/impl/d3d11_render_target_view.hpp>
#include<alya/graphics/core/details/impl/d3d11_depth_stencil_view.hpp>
#include<alya/graphics/core/details/impl/dxgi_format.hpp>
#include<alya/graphics/core/details/impl/d3d11_context.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_default_frame_buffer::d3d11_default_frame_buffer(
		native_window_handle_type window, 
		pixel_type back_buffer_pixel, 
		pixel_type depth_buffer_pixel, 
		size_t samples,
		windows::com::shared_ptr<ID3D11Device>device
	) : 
		swap_chain_(window, back_buffer_pixel, 3, device.query_interface<IDXGIDevice>()),
		samples_(samples),
		back_buffer_pixel_(back_buffer_pixel),
		depth_buffer_pixel_(depth_buffer_pixel)
	{
		resize_buffers(window_width(), window_height());
	}

	void d3d11_default_frame_buffer::resize_buffers(size_t width_, size_t height_)
	{
		attach_color_buffer(nullptr, 0);
		attach_depth_stencil_buffer(nullptr);
		back_buffer_ms_ = std::nullopt;
		swap_chain_.resize_buffers(width_, height_);
		
		auto back_buffer = swap_chain_.get_buffer<ID3D11Texture2D>(0);

		windows::com::shared_ptr<ID3D11Device> device;

		back_buffer->GetDevice(&device);

		if (samples() > 1)
		{
			d3d11_texture2d_array back_buffer_ms(width(), height(), 1, 1, samples(), false, back_buffer_pixel_, memory_qualifier::default_, texture_binding::color_buffer, device);
			d3d11_render_target_view back_buffer_ms_view(back_buffer_ms.native_handle());
			attach_color_buffer(back_buffer_ms_view.native_handle(), 0);
			if (depth_buffer_pixel_ != pixel_type::none)
			{
				d3d11_texture2d_array depth_buffer_ms(width(), height(), 1, 1, samples(), false, depth_buffer_pixel_, memory_qualifier::default_, texture_binding::depth_stencil_buffer, device);
				d3d11_depth_stencil_view depth_buffer_ms_view(depth_buffer_ms.native_handle());
				attach_depth_stencil_buffer(depth_buffer_ms_view.native_handle());
			}
			back_buffer_ms_.emplace(std::move(back_buffer_ms));
		}
		else
		{
			d3d11_render_target_view back_buffer_view(back_buffer, 0);
			attach_color_buffer(back_buffer_view.native_handle(), 0);
			if (depth_buffer_pixel_ != pixel_type::none)
			{
				d3d11_texture2d_array depth_buffer(width(), height(), 1, 1, 1, false, depth_buffer_pixel_, memory_qualifier::default_, texture_binding::depth_stencil_buffer, device);
				d3d11_depth_stencil_view depth_buffer_ms_view(depth_buffer.native_handle(), 0);
				attach_depth_stencil_buffer(depth_buffer_ms_view.native_handle());
			}
		}

	}

	size_t d3d11_default_frame_buffer::window_width()const noexcept
	{
		RECT rect{};
		GetClientRect(window(), &rect);
		return rect.right - rect.left;
	}

	size_t d3d11_default_frame_buffer::window_height()const noexcept
	{
		RECT rect{};
		GetClientRect(window(), &rect);
		return rect.bottom - rect.top;
	}

	void d3d11_default_frame_buffer::do_present(bool sync)noexcept
	{
		if (samples() > 1)
		{
			auto back_buffer = swap_chain_.get_buffer<ID3D11Texture2D>(0);

			windows::com::shared_ptr<ID3D11Device> device;

			back_buffer_ms_->native_handle()->GetDevice(&device);

			windows::com::shared_ptr<ID3D11DeviceContext> context;
			device->GetImmediateContext(&context);

			ALYA_GFX_CALL(context->ResolveSubresource(back_buffer.get(), 0, back_buffer_ms_->native_handle().get(), 0, dxgi_format(back_buffer_pixel_)));
			context->Flush();
		}

		swap_chain_.present(sync ? 1 : 0);

	}

}
