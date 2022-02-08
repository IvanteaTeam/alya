#pragma once
#include<alya/audio/core/sample.hpp>

namespace alya::audio::core
{

	template<sample Sample, size_t Channels>
	class wave_format
	{
	public:
		using sample_type = Sample;
		static const size_t channels = Channels;
	};

}
