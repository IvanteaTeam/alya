#pragma once
#include<alya/async/promise.hpp>
#include<alya/graphics/core/context_base.hpp>
#include<system_error>
#include<vector>

namespace alya::graphics::core
{
	class context_base;

	class vertex_shader
	{
	public:

		vertex_shader(const void*bytecode, size_t size, context_base&context)
			: impl_(bytecode, size, context.impl_)
		{}

		class loader
		{
		public:

			template<typename Reader, typename Executor>
			static async::promise<vertex_shader> async_load(std::string path, Reader reader, Executor ex, context_base&context)
			{
				auto data = co_await reader.async_read(path);
				auto data_begin = reinterpret_cast<const char*>(&*std::begin(data));
				auto data_end = reinterpret_cast<const char*>(&*--std::end(data) + 1);
				co_return vertex_shader(data_begin, data_end - data_begin, context);
			}
		};

	private:
		details::d3d11_vertex_shader impl_;
		template<typename, typename, size_t>
		friend class basic_context;
		template<typename...T>
		friend class vertex_stream;
	};

}
