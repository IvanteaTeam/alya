#include<alya/utility/windows/com/shared_ptr.hpp>
#include<Unknwn.h>

namespace alya::windows::com
{
	shared_ptr_base::shared_ptr_base(IUnknown* ptr)noexcept : ptr(nullptr)
	{
		reset(ptr);
	}

	shared_ptr_base::shared_ptr_base(shared_ptr_base&& other)noexcept : shared_ptr_base()
	{
		reset(other.ptr);
		other.ptr = nullptr;
	}

	shared_ptr_base::shared_ptr_base(shared_ptr_base const& other)noexcept : shared_ptr_base()
	{
		reset(other.ptr);
		if (ptr)
			ptr->AddRef();
	}

	shared_ptr_base::~shared_ptr_base()
	{
		reset();
	}

	shared_ptr_base& shared_ptr_base::operator=(shared_ptr_base&& other)noexcept
	{
		shared_ptr_base c(std::move(other));
		swap(c);
		return *this;
	}

	shared_ptr_base& shared_ptr_base::operator=(shared_ptr_base const& other)noexcept
	{
		shared_ptr_base c(other);
		swap(c);
		return *this;
	}

	void shared_ptr_base::reset(IUnknown* p)noexcept
	{
		if (ptr != nullptr)
			ptr->Release();
		ptr = p;
	}

	shared_ptr_base shared_ptr_base::query_interface(const _GUID&id)const noexcept
	{
		void*p;
		auto res = ptr->QueryInterface(id, &p);
		return reinterpret_cast<IUnknown*>(p);
	}

}
