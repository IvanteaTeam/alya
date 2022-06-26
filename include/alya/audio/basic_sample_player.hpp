#pragma once
#include<alya/audio/basic_sample.hpp>
#include<alya/audio/basic_channel.hpp>

namespace alya::audio
{

	template<typename WaveFormat>
	class basic_sample_player : public basic_channel<WaveFormat>
	{
	public:

		using wave_format = WaveFormat;
		using frame_type = core::frame<wave_format>;

		
		//basic_sample_player() : data_(nullptr), size_(0), offset_(0) {}
		/*
		template<typename A>
		void play(const basic_sample<wave_format, A>&sample)
		{
			offset_ = size_;
		}*/

		template<typename A>
		basic_sample_player(const basic_sample<wave_format, A>&s) : data_(s.data()), frames_(s.frames()), offset_(frames_), sample_rate_(s.sample_rate()) {}

		core::const_buffer<wave_format> get_data(size_t frames)noexcept override
		{
			size_t offset = offset_;
			if (data_ && offset < frames_)
			{
				size_t d = std::min(frames, frames_ - offset);
				auto tmp = offset;
				offset_.compare_exchange_strong(tmp, offset + d);
				return { data_ + offset, data_ + offset + d};
			}
			else
				return {};
		}

		void play() noexcept
		{
			offset_ = 0;
		}

		void stop() noexcept
		{
			offset_ = frames_;
		}

		size_t sample_rate()const noexcept override
		{
			return sample_rate_;
		}

		size_t is_playing()const noexcept
		{
			return offset_ < frames_;
		}

	private:
		const frame_type* data_;
		size_t frames_;
		size_t sample_rate_;
		std::atomic<size_t> offset_;
	};

}
