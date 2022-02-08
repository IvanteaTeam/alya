#pragma once
#include<alya/resource/async_load.hpp>
#include<alya/graphics/core/details/d3d11/object_base.hpp>
#include<system_error>

namespace alya::graphics::core
{

	class pixel_shader : protected d3d11::object_base
	{
	public:

		template<typename C>
		pixel_shader(const void* bytecode, size_t size, C& ctx)
			: pixel_shader(bytecode, size, get_device(ctx), get_device_context(ctx))
		{}

		class importer
		{
		public:

			template<resource::loader L, typename C>
			async::promise<pixel_shader> operator()(L& loader, typename L::source_type path, async::executor auto ex, C& ctx)
			{
				auto data = co_await resource::async_load(loader, std::move(path));
				co_return pixel_shader(data.data(), data.size(), ctx);
			}
		};

	private:
		pixel_shader(const void*, size_t, d3d11::device_ptr, d3d11::device_context_ptr);
		mutable d3d11::pixel_shader_ptr shader;
		friend class context_base;
	};

}
