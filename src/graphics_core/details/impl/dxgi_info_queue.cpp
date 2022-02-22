#include<alya/graphics/core/details/impl/dxgi_info_queue.hpp>
#include<dxgidebug.h>
#include<stdexcept>
#include<vector>

namespace alya::graphics::core::details
{

	size_t dxgi_info_queue::size()const noexcept
	{
		return impl_ ? impl_->GetNumStoredMessages(DXGI_DEBUG_ALL) : 0;
	}

	void dxgi_info_queue::clear()noexcept
	{
		impl_->ClearStoredMessages(DXGI_DEBUG_ALL);
	}

	std::string dxgi_info_queue::get(size_t index)const
	{
		if (index >= size())
			throw std::out_of_range{"Access to message with index out of range."};
		size_t size = 0;
		impl_->GetMessageA(DXGI_DEBUG_ALL, index, nullptr, &size);
		std::vector<char> data(size);

		auto* msg = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(data.data());
		impl_->GetMessageA(DXGI_DEBUG_ALL, index, msg, &size);
		return msg->pDescription;
	}

}
