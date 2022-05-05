#pragma once
#include<alya/audio/core/details/impl/wasapi_device_id.hpp>

namespace alya::audio::core
{
	
	class device_id
	{
	public:

		

	private:
		
		device_id(details::wasapi_device_id&&id)
			: impl_(std::move(id))
		{}

		details::wasapi_device_id impl_;
		friend class device_enumerator;
		friend class device;
	};

}
