#pragma once
#include<alya/ui/core/window_base.hpp>
#include<alya/async/execute.hpp>

namespace alya::ui::core
{

	template<typename Executor>
	class basic_window : public window_base
	{
	public:

		using executor_type = Executor;

		basic_window(const executor_type & ex= executor_type{}) : 
			executor_(ex), window_base()
		{}

		basic_window(int x, int y, int width, int height, const executor_type & ex = executor_type{}) : 
			executor_(ex), window_base(x, y, width, height)
		{}

		void set_event_callback(std::function<void(event_base*)>callback)noexcept
		{
			callback_ = std::move(callback);
		}

	private:

		virtual void handle_event(std::unique_ptr<event_base>e)noexcept override
		{
			async::execute(executor_, [e = std::move(e), this]() {
				
				callback_(e.get());
			});
		}
		std::function<void(event_base*)> callback_;
		executor_type executor_;
	};

}
