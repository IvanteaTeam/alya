#pragma once
#include<dxgidebug.h>
#include<string>
#include<alya/utility/windows/com/shared_ptr.hpp>

namespace alya::graphics::dxgi
{

	class info_queue
	{
	public:

		info_queue(info_queue&&) = delete;
		info_queue(const info_queue&) = delete;
		info_queue& operator=(info_queue&) = delete;
		info_queue& operator=(const info_queue&) = delete;

		std::string get(size_t index)const;
		size_t size()const noexcept;
		void clear()noexcept;

	private:

		info_queue(windows::com::shared_ptr<IDXGIInfoQueue>);

		mutable windows::com::shared_ptr<IDXGIInfoQueue> queue;
		friend info_queue& get_debug_messages();
	};

	info_queue& get_debug_messages();

}
