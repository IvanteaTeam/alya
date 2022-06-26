#pragma once
#include<alya/audio/core/details/impl/wasapi_endpoint.hpp>
#include<alya/audio/core/device.hpp>

namespace alya::audio::core
{

	class system_endpoint_base : public details::wasapi_endpoint
	{
	public:

		system_endpoint_base(const device& device_, details::sample_type sample, size_t channels, size_t sample_rate) :
			wasapi_endpoint(device_.impl_, sample, channels, sample_rate)
		{}

		~system_endpoint_base()
		{
			stop();
		}

	private:

	};

}
