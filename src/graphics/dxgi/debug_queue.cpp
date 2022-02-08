#include<alya/graphics/details/dxgi/debug_queue.hpp>
#include<stdexcept>
#include<vector>

namespace alya::graphics::dxgi
{
	

	info_queue::info_queue(windows::com::shared_ptr<IDXGIInfoQueue> queue)
		: queue(queue)
	{}

	size_t info_queue::size()const noexcept
	{
		return queue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	void info_queue::clear()noexcept
	{
		queue->ClearStoredMessages(DXGI_DEBUG_ALL);
	}

	std::string info_queue::get(size_t index)const
	{
		if (index >= size())
			throw std::out_of_range{"attempt to access element out of range"};
		size_t size = 0;
		queue->GetMessageA(DXGI_DEBUG_ALL, index, nullptr, &size);
		std::vector<char> data(size);

		auto* msg = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(data.data());
		queue->GetMessageA(DXGI_DEBUG_ALL, index, msg, &size);
		return msg->pDescription;
	}

	info_queue& get_debug_messages()
	{
		auto create = []() {
			windows::com::shared_ptr<IDXGIInfoQueue> queue;
			auto dxgi_dll = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
			if (!dxgi_dll)
				throw std::runtime_error{ "failed to load dxgi debug" };
			auto get = reinterpret_cast<decltype(DXGIGetDebugInterface)*>(GetProcAddress(dxgi_dll, "DXGIGetDebugInterface"));
			if (!get)
				throw std::runtime_error{ "failed to load dxgi debug" };
			auto ret = get(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&queue));
			if (ret != S_OK)
				throw std::runtime_error{ "failed to load dxgi debug" };
			return queue;
		};
		static info_queue queue = create();
		return queue;
	}

}
