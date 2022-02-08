#pragma once
#include<alya/audio/core/wave_format.hpp>

namespace alya::audio::core
{

	template<typename WaveFormat>
	class basic_buffer
	{
	public:

		using wave_format = WaveFormat;
		using sample_type = typename wave_format::sample_type;
		static const size_t channels = wave_format::channels;
		
		basic_buffer() noexcept : 
			data_(nullptr), 
			frames_(0) 
		{}
		
		basic_buffer(sample_type*data, size_t frames) noexcept :
			data_(data),
			frames_(frames)
		{}

		size_t frames()const noexcept
		{
			return frames_;
		}

		sample_type& get(size_t frame, size_t channel)noexcept
		{
			return data_[frame * channels + channel];
		}

		const sample_type& get(size_t frame, size_t channel)const noexcept
		{
			return data_[frame * channels + channel];
		}

	private:
		sample_type* data_;
		size_t frames_;
	};

}
