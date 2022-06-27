#pragma once
#include<alya/async/promise.hpp>
#include<alya/graphics/core/context_base.hpp>
#include<system_error>

namespace alya::graphics::core
{

	class context_base;

	class pixel_shader
	{
	public:

		pixel_shader(const void*bytecode, size_t size, context_base&context)
			: impl_(bytecode, size, context.impl_)
		{}

		class loader
		{
		public:
			using resource_type = pixel_shader;
			template<typename Data>
			static pixel_shader load(const Data& data, context_base& context)
			{
				return pixel_shader(data.data(), data.size(), context);
			}
		};

	private:
		details::d3d11_pixel_shader impl_;
		template<typename, typename, size_t>
		friend class basic_context;
	};

}
