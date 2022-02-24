#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>

struct ID3D11Device;

namespace alya::graphics::core::details
{

	class d3d11_device
	{
	public:

		using native_handle_type = windows::com::shared_ptr<ID3D11Device>;

		explicit d3d11_device();
		d3d11_device(const d3d11_device&) = delete;
		d3d11_device(d3d11_device&&) = default;
		d3d11_device& operator=(const d3d11_device&) = delete;
		d3d11_device& operator=(d3d11_device&&) = default;

		native_handle_type native_handle()const noexcept
		{
			return impl_;
		}

	private:
		windows::com::shared_ptr<ID3D11Device> impl_;
	};

}
