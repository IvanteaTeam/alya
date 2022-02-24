#include<alya/graphics/core/details/impl/d3d11_device.hpp>
#include<alya/graphics/core/details/debug.hpp>
#include<alya/utility/windows/com/intialize.hpp>
#include<d3d11.h>

namespace alya::graphics::core::details
{

	d3d11_device::d3d11_device()
	{
		windows::com::initialize();

		D3D_FEATURE_LEVEL feature = D3D_FEATURE_LEVEL_10_0;
		D3D_DRIVER_TYPE driver = D3D_DRIVER_TYPE_HARDWARE;

		int flags = 0;

#ifndef NDEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		windows::com::shared_ptr<ID3D11DeviceContext> context;

		ALYA_GFX_CALL(D3D11CreateDevice(nullptr, driver, nullptr, flags, &feature, 1, D3D11_SDK_VERSION, &impl_, nullptr, &context));
	}

}
