#pragma once
#include<alya/graphics/core/details/impl/d3d11_pixel_shader.hpp>
#include<system_error>

namespace alya::graphics::core
{

	class context_base;

	class pixel_shader
	{
	public:

		pixel_shader(const void*, size_t, context_base&);

		class importer
		{
		public:

			pixel_shader operator()(const auto& data, context_base& context)
			{
				auto data_begin = reinterpret_cast<const char*>(&*std::begin(data));
				auto data_end = reinterpret_cast<const char*>(&*--std::end(data) + 1);
				return pixel_shader(data_begin, data_end - data_begin, context);
			}
		};

	private:
		details::d3d11_pixel_shader impl_;
		template<typename, typename, size_t>
		friend class basic_context;
	};

}
