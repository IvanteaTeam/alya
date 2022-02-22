#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<string>

struct IDXGIInfoQueue;

namespace alya::graphics::core::details
{

	class dxgi_info_queue
	{
	public:

		explicit dxgi_info_queue(windows::com::shared_ptr<IDXGIInfoQueue>impl)noexcept : impl_(impl) {}
		dxgi_info_queue(const dxgi_info_queue&) = delete;
		dxgi_info_queue(dxgi_info_queue&&) = default;
		dxgi_info_queue& operator=(const dxgi_info_queue&) = delete;
		dxgi_info_queue& operator=(dxgi_info_queue&&) = default;

		size_t size()const noexcept;
		void clear()noexcept;
		std::string get(size_t)const;

	private:
		mutable windows::com::shared_ptr<IDXGIInfoQueue> impl_;
	};

}
