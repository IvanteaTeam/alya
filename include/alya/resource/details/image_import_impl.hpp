#pragma once
#include<memory>
#include<array>

namespace alya::resource::details
{

	std::tuple<void*, size_t, size_t, size_t> image_import_impl(
		const void*data, 
		size_t size, 
		void*(*malloc)(size_t)noexcept,
		void*(*realloc)(void*, size_t)noexcept,
		void(*free)(void*)noexcept, 
		size_t components,
		bool flipped
	)noexcept;

	template<typename A>
	std::tuple<typename A::value_type*, size_t, size_t> image_import_impl(const void*data, size_t size, A&alloc_, size_t components, bool flipped)
	{
		using T = typename std::allocator_traits<A>::value_type;
		thread_local A* al = &alloc_;
		thread_local std::exception_ptr curr_exception;
		thread_local std::array<std::pair<void*, size_t>, 100> allocs_{};
		thread_local auto allocs = allocs_.begin();
		al = &alloc_;
		allocs_.fill({ nullptr, 0 });
		allocs = allocs_.begin();
		curr_exception = nullptr;

		static auto malloc = [](size_t size) noexcept -> void* 
		{
			if (curr_exception)
				return nullptr;
			try
			{
				size_t count = (size + sizeof(T) - 1) / sizeof(T);
				auto ptr = al->allocate(count);
				*allocs = { ptr, count * sizeof(T) };
				allocs++;
				return ptr;
			}
			catch (...)
			{
				curr_exception = std::current_exception();
				return nullptr;
			}
		};
		static auto find = [](void*ptr)noexcept {
			auto it = std::find_if(allocs_.begin(), allocs_.end(), [&](auto a) { return a.first == ptr; });
			assert(ptr && it != allocs_.end());
			return it;
		};
		static auto size_of = [](void*ptr)noexcept
		{
			auto it = find(ptr);
			return it->second;
		};
		
		static auto free = [](void*ptr)noexcept 
		{
			if (!ptr)
				return;
			size_of(ptr);
			al->deallocate(static_cast<T*>(ptr), size_of(ptr));
			find(ptr)->first = nullptr;
		};

		static auto realloc = [](void*ptr, size_t size) noexcept->void*
		{
			if (!ptr)
				return malloc(size);
			auto s = size_of(ptr);
			auto p = malloc(size);
			if (!p)
				return nullptr;
			std::memcpy(p, ptr, std::min(s, size));
			free(ptr);
			return p;
		};

		auto [image, width, height, org_channels] = image_import_impl(data, size, malloc, realloc, free, components, flipped);
		if (curr_exception)
		{
			if (image)
				free(image);
			std::rethrow_exception(curr_exception);
		}
		return { static_cast<T*>(image), width, height };
	}

}
