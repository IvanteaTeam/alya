#pragma once
#include<alya/audio/core/basic_context.hpp>
#include<alya/audio/core/device_enumerator.hpp>
#include<boost/lockfree/queue.hpp>
#include<alya/audio/buffer_iterator.hpp>
#include<alya/async/execute.hpp>

namespace alya::audio
{

	template<typename WaveFormat, typename Executor>
	class basic_engine
	{
	public:

		using wave_format = WaveFormat;
		using executor_type = Executor;

		basic_engine(size_t init_sample_rate = 44100, executor_type ex = executor_type{}) :
			sample_rate_(init_sample_rate),
			executor_(ex),
			buffer_queue_(100)
		{
			update_device();
			device_enumerator_.set_device_list_callback([this]() {
				
				async::execute(executor_, [this]() {
					update_device();
				});
				
			}, core::default_device_changed);
		}

		void set_sample_rate(size_t rate)
		{
			if (context_)
				context_->set_sample_rate(rate);
			sample_rate_ = rate;
		}

		size_t get_sample_rate()const noexcept
		{
			return sample_rate_;
		}

		void async_play(buffer_iterator<wave_format> it)
		{
			std::lock_guard lock(new_buffers_mutex_);
			
			if (auto free = find_empty_buffer_iterator(new_buffers_.begin(), new_buffers_.end()); free != new_buffers_.end())
			{
				*free = std::move(it);
				buffer_queue_.push(&*free);
			}
			else
			{
				auto& it_ref = new_buffers_.emplace_back(std::move(it));
				buffer_queue_.push(&it_ref);
			}
		}

		executor_type get_executor()const
		{
			return executor_;
		}

	private:

		void update_device()noexcept
		{
			try {
				auto device = device_enumerator_.get_default_device();
				if (device)
				{
					context_.emplace(*device, sample_rate_);
					context_->set_fill_buffer_callback([this](auto buffer) {
						fill_buffer(buffer);
						});
					context_->start();
				}
			}
			catch (const std::exception& e)
			{

			}
		}

		void fill_buffer(core::basic_buffer<wave_format> buffer)noexcept
		{
			
			buffer_iterator<wave_format>* it;

			while (buffer_queue_.pop(it))
			{
				auto free = find_empty_buffer_iterator(processing_buffers_.begin(), processing_buffers_.end());
				if (free != processing_buffers_.end())
					*free = std::move(*it);
				else
					processing_buffers_.emplace_back(std::move(*it));
			}
			

			for (size_t i = 0; i < buffer.frames(); i++)
			{
				buffer.get(i, 0) = 0;
				buffer.get(i, 1) = 0;
				for (auto& it : processing_buffers_)
				{
					if (!it)
						continue;
					auto frame = *it;
					for (size_t j = 0; j < wave_format::channels; j++)
						buffer.get(i, j) += frame[j];
					++it;
				}
			}

		}

		static auto find_empty_buffer_iterator(auto begin, auto end)
		{
			return std::find_if(begin, end, [](auto& it) { return !bool(it); });
		}

		std::vector<buffer_iterator<wave_format>> processing_buffers_;
		boost::lockfree::queue<buffer_iterator<wave_format>*> buffer_queue_;
		std::mutex new_buffers_mutex_;
		std::list<buffer_iterator<wave_format>> new_buffers_;

		size_t sample_rate_;
		std::optional<core::basic_context<wave_format>> context_;
		core::device_enumerator device_enumerator_;
		executor_type executor_;
	};

}
