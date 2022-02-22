#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>

struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

namespace alya::graphics::core::details
{

	class d3d11_render_target_view
	{
	public:
		d3d11_render_target_view(windows::com::shared_ptr<ID3D11Texture2D>);
		d3d11_render_target_view(windows::com::shared_ptr<ID3D11Texture2D>, size_t);
		d3d11_render_target_view(const d3d11_render_target_view&) = delete;
		d3d11_render_target_view(d3d11_render_target_view&&) = default;
		d3d11_render_target_view& operator=(const d3d11_render_target_view&) = delete;
		d3d11_render_target_view& operator=(d3d11_render_target_view&&) = default;

		windows::com::shared_ptr<ID3D11RenderTargetView> native_handle()const noexcept
		{
			return impl_;
		}

	private:
		windows::com::shared_ptr<ID3D11RenderTargetView> impl_;
	};

}
