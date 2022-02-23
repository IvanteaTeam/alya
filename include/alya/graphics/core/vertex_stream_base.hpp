#pragma once
//#include<alya/graphics/core/attribute_stream.hpp>
#include<alya/graphics/core/details/impl/d3d11_input_layout.hpp>

namespace alya::graphics::core
{
	
	class context_base;
	class vertex_shader;

	class vertex_stream_base
	{
	public:

		vertex_stream_base(std::initializer_list<details::attribute_signature>signatures, const vertex_shader&shader, context_base& ctx);
		
	private:
		details::d3d11_input_layout input_layout_;	
		friend class context_base;
	};

}
