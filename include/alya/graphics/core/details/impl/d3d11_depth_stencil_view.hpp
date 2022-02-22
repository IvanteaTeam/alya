#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>

struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

namespace alya::graphics::core::details
{

	class d3d11_depth_stencil_view
	{
	public:
		d3d11_depth_stencil_view(windows::com::shared_ptr<ID3D11Texture2D>);
		d3d11_depth_stencil_view(windows::com::shared_ptr<ID3D11Texture2D>, size_t);
		d3d11_depth_stencil_view(const d3d11_depth_stencil_view&) = delete;
		d3d11_depth_stencil_view(d3d11_depth_stencil_view&&) = default;
		d3d11_depth_stencil_view& operator=(const d3d11_depth_stencil_view&) = delete;
		d3d11_depth_stencil_view& operator=(d3d11_depth_stencil_view&&) = default;

		windows::com::shared_ptr<ID3D11DepthStencilView> native_handle()const noexcept
		{
			return impl_;
		}

	private:
		windows::com::shared_ptr<ID3D11DepthStencilView> impl_;
	};

}
