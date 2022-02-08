#pragma once
//#include<alya/gl/program.hpp>
//#include<alya/gl/texture.hpp>
#include<string>

#include<alya/graphics/context.hpp>
#include<alya/graphics/texture2d.hpp>
#include<alya/graphics/constant_buffer.hpp>
#include<alya/graphics/vertex_stream.hpp>

#include<glm/glm.hpp>
#include<glm/ext.hpp>


using namespace alya;

extern std::atomic<bool> end;

namespace gfx = alya::graphics;

namespace std
{

	template<typename T>
	struct tuple_size<glm::tvec4<T>> : std::integral_constant<size_t, 4> {};

	template<typename T>
	struct tuple_size<glm::tvec3<T>> : std::integral_constant<size_t, 3> {};

	template<typename T>
	struct tuple_size<glm::tvec2<T>> : std::integral_constant<size_t, 2> {};

	template<typename T>
	struct tuple_size<glm::tvec1<T>> : std::integral_constant<size_t, 1> {};



}

namespace alya::graphics
{


	template<size_t N, typename T, glm::qualifier Q>
	struct constant_buffer_variable_type<glm::mat<N, N, T, Q>>
	{
		using type = matrix<T, N>;
	};

}
