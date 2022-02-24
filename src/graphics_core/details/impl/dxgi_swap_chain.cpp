#include<alya/graphics/core/details/impl/dxgi_swap_chain.hpp>
#include<alya/graphics/core/details/impl/dxgi_format.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	dxgi_swap_chain::dxgi_swap_chain(native_window_handle_type window, details::pixel_type pixel, size_t buffers_count, windows::com::shared_ptr<IDXGIDevice>device)
		: buffers_(buffers_count), pixel_(pixel), window_(window)
	{
		
		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferCount = buffers();
		desc.BufferDesc.Width = 0;
		desc.BufferDesc.Height = 0;
		desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		desc.BufferDesc.Format = details::dxgi_format(pixel);
		desc.BufferDesc.RefreshRate.Numerator = 0;
		desc.BufferDesc.RefreshRate.Denominator = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = window;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Windowed = true;

		windows::com::shared_ptr<IDXGIAdapter> adapter;
		windows::com::shared_ptr<IDXGIFactory> factory;
		device->GetParent(__uuidof(*adapter.get()), reinterpret_cast<void**>(&adapter));
		adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

		ALYA_GFX_CALL(factory->CreateSwapChain(device.get(), &desc, &swap_chain_));

	}

	void dxgi_swap_chain::present(size_t sync_interval)noexcept
	{
		ALYA_GFX_CALL(swap_chain_->Present(sync_interval, 0));
	}

	void dxgi_swap_chain::do_resize_buffers(size_t width, size_t height)
	{
		width_ = width;
		height_ = height;
		ALYA_GFX_CALL(swap_chain_->ResizeBuffers(buffers(), width, height, details::dxgi_format(pixel_), 0));
	}

	void dxgi_swap_chain::get_buffer(size_t index, const UUID& id, void**resource)const noexcept
	{
		ALYA_GFX_CALL(swap_chain_->GetBuffer(index, id, resource));
	}

}
