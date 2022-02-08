#pragma once
#include<alya/audio/core/basic_buffer.hpp>
#include<array>

namespace alya::audio
{

	template<typename WaveFormat>
	class buffer_iterator
	{
	public:

		using wave_format = WaveFormat;
		using sample_type = typename wave_format::sample_type;
		static const size_t channels = wave_format::channels;
		using frame_type = std::array<sample_type, channels>;
		using buffer_type = core::basic_buffer<wave_format>;

		buffer_iterator() : 
			curr_padding_(0)
		{}

		buffer_iterator(const buffer_iterator&) = delete;
		buffer_iterator(buffer_iterator&&) = default;
		buffer_iterator& operator=(const buffer_iterator&) = delete;
		buffer_iterator& operator=(buffer_iterator&&) = default;
		~buffer_iterator()
		{
			if(new_buffer_callback_)
				new_buffer_callback_(true);
		}

		explicit buffer_iterator(std::function<buffer_type(bool)> new_buffer_callback) noexcept : 
			new_buffer_callback_(std::move(new_buffer_callback)),
			curr_padding_(0)
		{
			update_buffer();
		}

		operator bool()const noexcept
		{
			return bool(new_buffer_callback_);
		}

		frame_type operator*()const noexcept
		{
			if (curr_padding_ >= curr_buffer_.frames())
				return frame_type{};
			frame_type frame{};
			for (size_t i = 0; i < channels; i++)
				frame[i] = curr_buffer_.get(curr_padding_, i);
			return frame;
		}

		void operator++()noexcept
		{
			curr_padding_++;
			if (curr_padding_ >= curr_buffer_.frames())
				update_buffer();	
		}

	private:

		void update_buffer()noexcept
		{
			if (new_buffer_callback_) {
				curr_buffer_ = new_buffer_callback_(false);
				curr_padding_ = 0;
				if (curr_buffer_.frames() == 0)
					*this = buffer_iterator{};
			}
		}

		std::function<buffer_type(bool)> new_buffer_callback_;
		buffer_type curr_buffer_;
		size_t curr_padding_;
	};

}
