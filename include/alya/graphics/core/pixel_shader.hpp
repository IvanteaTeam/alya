#pragma once
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

			template<typename Context>
			pixel_shader operator()(const auto& data, Context& ctx)
			{
				auto data_begin = reinterpret_cast<const char*>(&*std::begin(data));
				auto data_end = reinterpret_cast<const char*>(&*--std::end(data) + 1);
				return pixel_shader(data_begin, data_end - data_begin, ctx);
			}
		};

	private:
		pixel_shader(const void*, size_t, d3d11::device_ptr, d3d11::device_context_ptr);
		mutable d3d11::pixel_shader_ptr shader;
		friend class context_base;
	};

}
