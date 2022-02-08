#pragma once
#include<memory>
#include<alya/resource/async_load.hpp>
#include<alya/async/execute.hpp>
#include<alya/audio/basic_decoder.hpp>
#include<alya/resource/bad_format.hpp>
#include<alya/audio/basic_engine.hpp>

namespace alya::audio
{
	
	template<typename WaveFormat, typename Allocator = std::allocator<typename WaveFormat::sample_type>>
	class basic_sample
	{
	public:

		using wave_format = WaveFormat;
		using allocator_type = Allocator;
		using sample_type = typename wave_format::sample_type;
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

		class importer;

	private:

		basic_sample(sample_type*data, size_t frames, size_t sample_rate, const allocator_type&alloc) : 
			data_(data),
			frames_(frames),
			sample_rate_(sample_rate),
			allocator_(alloc)
		{}

		sample_type* data_;
		size_t frames_;
		size_t sample_rate_;
		allocator_type allocator_;

		template<typename WaveFormat, typename Executor, typename Allocator>
		friend async::promise<void> async_play(basic_engine<WaveFormat, Executor>&, const basic_sample<WaveFormat, Allocator>&);
	};

	template<typename WaveFormat, typename Allocator>
	class basic_sample<WaveFormat, Allocator>::importer
	{
	public:

		using wave_format = WaveFormat;
		using allocator_type = Allocator;
		using sample_type = typename wave_format::sample_type;
		static const size_t channels = wave_format::channels;
		
		template<resource::loader L>
		async::promise<basic_sample<wave_format, allocator_type>> operator()(L& loader, typename L::source_type source, async::executor auto ex, const allocator_type & alloc = allocator_type{})
		{
			co_return co_await async::execute(ex, [data = co_await resource::async_load(loader, source), alloc = alloc]()mutable {
				basic_decoder<sample_type> decoder;
				std::error_code e;
				decoder.open(data.begin(), data.end(), e);
				if (e)
					throw resource::bad_format{};
				if (decoder.channels() != channels)
					throw resource::bad_format{};
				size_t buffer_size = decoder.frames() * channels;
				auto buffer = alloc.allocate(buffer_size);
				decoder.decode_some(buffer, buffer + buffer_size, e);
				if (e)
				{
					alloc.deallocate(buffer, buffer_size);
					throw resource::bad_format{};
				}
				return basic_sample(buffer, decoder.frames(), decoder.sample_rate(), alloc);
			});
		}

	};

	template<typename WaveFormat, typename Executor, typename Allocator>
	async::promise<void> async_play(basic_engine<WaveFormat, Executor>&engine, const basic_sample<WaveFormat, Allocator>&sample)
	{
		auto [p, d] = async::make_promise<void>();

		engine.async_play(buffer_iterator<WaveFormat>([was_got = false, data = sample.data_, frames = sample.frames(), d = std::move(d), &engine](bool end_of_play)mutable->core::basic_buffer<WaveFormat> {
			auto set = [&]() {
				async::execute(engine.get_executor(), [d = std::move(d)]()mutable {
					d.set_value();
				});
			};
			if(end_of_play)
			{
				set();
				return {};
			}
			if (was_got)
			{
				set();
				return {};
			}
			else
			{
				was_got = true;
				return { data, frames };
			}
		}));

		return std::move(p);
	}

}
