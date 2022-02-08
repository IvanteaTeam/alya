#include<combaseapi.h>
#include<alya/utility/windows/com/create_instance.hpp>
#include<alya/utility/windows/win32_error.hpp>

namespace alya::windows::com::details
{ 
				
	void* create_instance_impl(const _GUID& class_iid, const _GUID& interface_iid, IUnknown* outer)
	{
		void* out = nullptr;
		auto res = CoCreateInstance(class_iid, outer, CLSCTX_INPROC_SERVER, interface_iid, &out);
		if (res != S_OK)
			throw std::system_error{ make_error_code(res) };
		return out;
	}

}
