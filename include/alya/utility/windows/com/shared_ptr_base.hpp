#pragma once
#include<utility>

struct IUnknown;
struct _GUID;

namespace alya::windows::com
{
	class shared_ptr_base
	{
	public:
		shared_ptr_base(IUnknown* = nullptr)noexcept;
		shared_ptr_base(shared_ptr_base const&)noexcept;
		shared_ptr_base(shared_ptr_base&&)noexcept;
		shared_ptr_base& operator=(shared_ptr_base&&)noexcept;
		shared_ptr_base& operator=(shared_ptr_base const&)noexcept;
		~shared_ptr_base()noexcept;

		void reset(IUnknown* = nullptr)noexcept;
		

		constexpr IUnknown* get() noexcept
		{
			return ptr;
		}
		constexpr const IUnknown* get()const noexcept
		{
			return ptr;
		}

		constexpr operator bool()const noexcept
		{
			return get() != nullptr;
		}

	protected:
		constexpr void swap(shared_ptr_base&other)noexcept
		{
			std::swap(ptr, other.ptr);
		}
		constexpr IUnknown** address()
		{
			return &ptr;
		}
		constexpr const IUnknown** address()const
		{
			return const_cast<const IUnknown**>(&ptr);
		}
		shared_ptr_base query_interface(const _GUID&)const noexcept;
	private:
		IUnknown* ptr;
	};

	constexpr bool operator==(shared_ptr_base const& a, shared_ptr_base const& b)noexcept
	{
		return a.get() == b.get();
	}

	constexpr bool operator==(shared_ptr_base const& a, std::nullptr_t)noexcept
	{
		return !a;
	}

	constexpr bool operator==(std::nullptr_t, shared_ptr_base const& a)noexcept
	{
		return !a;
	}

	constexpr bool operator!=(shared_ptr_base const& a, shared_ptr_base const& b)noexcept
	{
		return a.get() != b.get();
	}

	constexpr bool operator!=(shared_ptr_base const& a, std::nullptr_t)noexcept
	{
		return a;
	}

	constexpr bool operator!=(std::nullptr_t, shared_ptr_base const& a)noexcept
	{
		return a;
	}

}
