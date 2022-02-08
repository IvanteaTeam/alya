#pragma once
#include<alya/utility/windows/com/shared_ptr_base.hpp>

namespace alya::windows::com
{

	template<typename T>
	class shared_ptr : public shared_ptr_base
	{
	public:
		constexpr shared_ptr(T* ptr = nullptr)noexcept : shared_ptr_base(ptr) {}
		shared_ptr(shared_ptr const&) noexcept = default;
		shared_ptr(shared_ptr&&) noexcept = default;
		shared_ptr& operator=(shared_ptr const&)noexcept = default;
		shared_ptr& operator=(shared_ptr&&) noexcept = default;

		constexpr T* get()noexcept { return reinterpret_cast<T*>(shared_ptr_base::get()); }
		constexpr const T* get()const noexcept { return reinterpret_cast<const T*>(shared_ptr_base::get()); }

		constexpr T* operator*()noexcept
		{
			return get();
		}

		constexpr const T* operator*()const noexcept
		{
			return get();
		}

		constexpr T* operator->()noexcept
		{
			return get();
		}

		constexpr const T* operator->()const noexcept
		{
			return get();
		}

		constexpr const T** address()const
		{
			return reinterpret_cast<const T**>(shared_ptr_base::address());
		}

		constexpr T** address()
		{
			return reinterpret_cast<T**>(shared_ptr_base::address());
		}

		constexpr T** operator&()
		{
			reset();
			return address();
		}

		template<typename T>
		shared_ptr<T> query_interface()const noexcept
		{
			auto ptr = shared_ptr_base::query_interface(__uuidof(T));
			return shared_ptr<T>{ ptr };
		}

		void swap(shared_ptr& other)noexcept
		{
			shared_ptr_base::swap(other);
		}

	private:
		explicit shared_ptr(const shared_ptr_base& other) noexcept : shared_ptr_base(other) {}
		template<typename U>
		friend class shared_ptr;
	};

}
