#include<alya/graphics/core/details/impl/dxgi_debug_queue.hpp>
#include<dxgidebug.h>

namespace alya::graphics::core::details
{

	static windows::com::shared_ptr<IDXGIInfoQueue> make_dxgi_debug_queue()noexcept
	{
		windows::com::shared_ptr<IDXGIInfoQueue> queue;
		auto dxgi_dll = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
		if (!dxgi_dll)
			return nullptr;
		auto get = reinterpret_cast<decltype(DXGIGetDebugInterface)*>(GetProcAddress(dxgi_dll, "DXGIGetDebugInterface"));
		if (!get)
			return nullptr;
		auto ret = get(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&queue));
		if (ret != S_OK)
			return nullptr;
		return queue;
	}

	dxgi_info_queue& dxgi_debug_queue()noexcept
	{
		static dxgi_info_queue queue{make_dxgi_debug_queue()};
		return queue;
	}

}
