#pragma once
#include<memory>
#include<alya/utility/unique_handle.hpp>
#include<alya/ui/core/event_base.hpp>

struct HWND__;

namespace alya::ui::core
{

	namespace details
	{
		class window_class;
	}

	class window_base
	{
	public:

		using native_handle_type = HWND__*;

		window_base() : window_base(cw_use_default, cw_use_default, cw_use_default, cw_use_default) {}
		window_base(const window_base&&) = delete;
		window_base(window_base&&) = delete;
		window_base& operator=(const window_base&) = delete;
		window_base& operator=(window_base&&) = delete;

		window_base(int x, int y, int width, int height);
		
		void open()noexcept;
		void restore()noexcept;
		void maximize()noexcept;
		void minimize()noexcept;
		void close()noexcept;

		bool is_open()const noexcept;
		bool has_focus()const noexcept;

		int width()const noexcept;
		int height()const noexcept;
		int client_width()const noexcept;
		int client_height()const noexcept;

		native_handle_type native_handle()noexcept;

	private:

		virtual void handle_event(std::unique_ptr<event_base>)noexcept = 0;

		void show(int)noexcept;
		void register_raw_input_device();

		int64_t handle_message(uint32_t, uint64_t, int64_t)noexcept;

		static const details::window_class& get_window_class();
		static int64_t __stdcall window_procedure(native_handle_type, uint32_t, uint64_t, int64_t);
		static const int cw_use_default = ((int)0x80000000);

		alya::unique_handle<native_handle_type, void(*)(native_handle_type)> handle_;
		bool is_open_;
		bool has_focus_;
	};

}
