#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>
#include<alya/utility/windows/com/intialize.hpp>

struct _GUID;

namespace alya::windows::com
{
	namespace details {
		void* create_instance_impl(const _GUID& class_iid, const _GUID& interface_iid, IUnknown* outer);
	}

	template<typename I>
	shared_ptr<I> create_instance(const _GUID&class_iid, IUnknown*outer = nullptr)
	{
		initialize();
		void* res = details::create_instance_impl(class_iid, __uuidof(I), outer);
		return shared_ptr{ reinterpret_cast<I*>(res) };
	}

}
