#pragma once
#include<tuple>

namespace alya::resource::details {
	thread_local void* (*malloc)(size_t)noexcept;
	thread_local void* (*realloc)(void*, size_t)noexcept;
	thread_local void(*free)(void*)noexcept;
}

#define STBI_NO_STDIO

#define STBI_MALLOC alya::resource::details::malloc
#define STBI_FREE alya::resource::details::free
#define STBI_REALLOC alya::resource::details::realloc

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

namespace alya::resource::details
{
	
	std::tuple<void*, size_t, size_t, size_t> image_import_impl(
		const void* data,
		size_t size,
		void* (*malloc_)(size_t)noexcept,
		void* (*realloc_)(void*, size_t)noexcept,
		void(*free_)(void*)noexcept,
		size_t components,
		bool flipped
	)noexcept
	{
		malloc = malloc_;
		realloc = realloc_;
		free = free_;
		int x, y, c;
		stbi_set_flip_vertically_on_load_thread(flipped);
		auto image = stbi_load_from_memory(static_cast<const stbi_uc*>(data), size, &x, &y, &c, components);
		return {image, x, y, c};
	}

}
