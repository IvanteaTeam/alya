#include<alya/audio/core/details/impl/wasapi_device.hpp>

namespace alya::audio::core::details
{

	wasapi_device::wasapi_device(windows::com::shared_ptr<IMMDevice>device)
		: device_(device)
	{}

}
