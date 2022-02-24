#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<alya/graphics/core/details/pixel_type.hpp>

struct IDXGIDevice;
struct IDXGISwapChain;

struct HWND__;

namespace alya::graphics::core::details
{

	class dxgi_swap_chain
	{
	public:

		using native_window_handle_type = HWND__*;
		static constexpr size_t min_width = 8, min_height = 8;

		dxgi_swap_chain(native_window_handle_type window, details::pixel_type pixel, size_t buffers_count, windows::com::shared_ptr<IDXGIDevice>device);
		dxgi_swap_chain(const dxgi_swap_chain&) = delete;
		dxgi_swap_chain(dxgi_swap_chain&&) = default;
		dxgi_swap_chain& operator=(const dxgi_swap_chain&) = delete;
		dxgi_swap_chain& operator=(dxgi_swap_chain&&) = default;

		void resize_buffers(size_t width, size_t height)
		{
			do_resize_buffers((std::max)(width, min_width), (std::max)(height, min_height));
		}

		size_t buffers()const noexcept
		{
			return buffers_;
		}

		template<typename Resource>
		windows::com::shared_ptr<Resource> get_buffer(size_t index)const noexcept
		{
			if (index >= buffers())
				return nullptr;
			windows::com::shared_ptr<Resource> resource;
			get_buffer(index, __uuidof(Resource), reinterpret_cast<void**>(&resource));
			return resource;
		}

		void present(size_t sync_interval)noexcept;

		size_t width()const noexcept
		{
			return width_;
		}

		size_t height()const noexcept
		{
			return height_;
		}

		native_window_handle_type window()const noexcept
		{
			return window_;
		}

	private:

		void do_resize_buffers(size_t, size_t);

		void get_buffer(size_t, const _GUID&, void**)const noexcept;

		details::pixel_type pixel_;
		size_t buffers_, width_, height_;
		mutable windows::com::shared_ptr<IDXGISwapChain> swap_chain_;
		native_window_handle_type window_;
	};

}
