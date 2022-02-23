#pragma once
#include<alya/graphics/core/details/impl/d3d11_vertex_shader.hpp>
#include<system_error>
#include<vector>

namespace alya::graphics::core
{
	class context_base;

	class vertex_shader
	{
	public:

		vertex_shader(const void*, size_t, context_base&);

		class importer
		{
		public:

			vertex_shader operator()(const auto&data, context_base&context)
			{
				auto data_begin = reinterpret_cast<const char*>(&*std::begin(data));
				auto data_end = reinterpret_cast<const char*>(&*--std::end(data) + 1);
				return vertex_shader(data_begin, data_end - data_begin, context);
			}
		};

	private:
		details::d3d11_vertex_shader impl_;
		friend class context_base;
		friend class vertex_stream_base;
	};

}
