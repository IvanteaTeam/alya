#pragma once
#include<memory>
#include<alya/async/promise.hpp>
#include<alya/audio/basic_decoder.hpp>
#include<alya/resource/bad_format.hpp>
#include<alya/audio/core/frame.hpp>

namespace alya::audio
{
	
	template<typename WaveFormat, typename Allocator = std::allocator<core::frame<WaveFormat>>>
	class basic_sample
	{
	public:

		using wave_format = WaveFormat;
		using allocator_type = Allocator;
		using frame_type = core::frame<wave_format>;
		static const size_t channels = wave_format::channels;

		basic_sample(const allocator_type& alloc = allocator_type{}) noexcept(std::is_nothrow_copy_constructible_v<allocator_type>) : 
			allocator_(alloc),
			data_(nullptr),
			frames_(0),
			sample_rate_(0)
		{}

		basic_sample(const basic_sample&) = delete;
		basic_sample(basic_sample&&other) noexcept(std::is_nothrow_move_constructible_v<allocator_type>) : 
			allocator_(std::move(other.allocator_)),
			data_(other.data_),
			frames_(other.frames_),
			sample_rate_(other.sample_rate_)
		{
			other.data_ = nullptr;
		}

		basic_sample& operator=(const basic_sample&) = delete;
		basic_sample& operator=(basic_sample&& other) noexcept(std::is_nothrow_move_assignable_v<allocator_type>)
		{
			clear();
			allocator_ = std::move(other.allocator_);
			data_ = other.data_;
			frames_ = other.frames_;
			sample_rate_ = other.sample_rate_;
			other.data_ = nullptr;
			return *this;
		}
		~basic_sample()
		{
			clear();
		}

		size_t sample_rate()const noexcept
		{
			return sample_rate_;
		}

		size_t frames()const noexcept
		{
			return frames_;
		}

		void clear()noexcept
		{
			if (data_)
				allocator_.deallocate(data_, frames_ * channels);
			data_ = nullptr;
		}

		const frame_type* data()const noexcept
		{
			return data_;
		}

		class loader;

	private:

		basic_sample(frame_type*data, size_t frames, size_t sample_rate, const allocator_type&alloc) : 
			data_(data),
			frames_(frames),
			sample_rate_(sample_rate),
			allocator_(alloc)
		{}

		frame_type* data_;
		size_t frames_;
		size_t sample_rate_;
		allocator_type allocator_;
	};

	template<typename WaveFormat, typename Allocator>
	class basic_sample<WaveFormat, Allocator>::loader
	{
	public:

		using wave_format = WaveFormat;
		using allocator_type = Allocator;
		using sample_type = typename wave_format::sample_type;
		using resource_type = basic_sample<wave_format, allocator_type>;
		static const size_t channels = wave_format::channels;
		
		template<typename Reader, typename Ex>
		static async::promise<basic_sample<wave_format, allocator_type>> async_load(std::string path, Reader reader, Ex ex, allocator_type alloc = allocator_type{})
		{
			auto data = co_await reader.async_read(path);
			basic_decoder<sample_type> decoder;
			std::error_code e;
			decoder.open(std::begin(data), std::end(data), e);
			if (e)
				throw resource::bad_format{};
			if (decoder.channels() != channels)
				throw resource::bad_format{};
			
			auto buffer = alloc.allocate(decoder.frames());
			decoder.decode_some(reinterpret_cast<sample_type*>(buffer), reinterpret_cast<sample_type*>(buffer + decoder.frames()), e);
			if (e)
			{
				alloc.deallocate(buffer, decoder.frames());
				throw resource::bad_format{};
			}
			co_return basic_sample(buffer, decoder.frames(), decoder.sample_rate(), alloc);
		}

	};


}
