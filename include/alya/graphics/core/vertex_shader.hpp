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
			using resource_type = vertex_shader;
			
			template<typename Data>
			static vertex_shader load(const Data& data, context_base& context)
			{
				return vertex_shader(data.data(), data.size(), context);
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
