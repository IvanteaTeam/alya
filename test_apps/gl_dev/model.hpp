#pragma once
//#include<alya/gl/shader.hpp>
//#include<alya/gl/buffer.hpp>
//#include<alya/gl/vertex_array.hpp>
//#include<alya/gl/texture.hpp>
#include"help.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<vector>

#include<alya/graphics/core/vertex_buffer.hpp>
#include<alya/graphics/core/vertex_shader.hpp>

namespace dev {
	
	using namespace alya;

	class Mesh
	{
	private:

		graphics::core::context_base& ctx;
		graphics::core::vertex_buffer<glm::vec3, graphics::core::memory_qualifier::immutable> positions;
		graphics::core::vertex_buffer<glm::vec2, graphics::core::memory_qualifier::immutable> tex_coords;
		size_t vertices_count;
		glm::mat4 transform;
		glm::vec3 color;
		glm::vec3 center;

		friend class Model;
	public:
		Mesh(graphics::core::context_base& ctx, std::vector<glm::vec3>const& vetices, std::vector<glm::vec2>const& tex_coords);
		Mesh(Mesh&&) = default;
		Mesh(Mesh const&) = delete;
		Mesh& operator=(Mesh&&) = delete;
		Mesh& operator=(Mesh const&) = delete;

		inline void set_transform(glm::mat4 const&transform)
		{
			this->transform = transform;
		}

		inline glm::mat4 get_transform()
		{
			return transform;
		}

		inline void set_color(glm::vec3 color)
		{
			this->color = color;
		}

		inline glm::vec3 get_color()
		{
			return color;
		}
		void render();
	};

	class Model
	{
	public:
		using cb_t = std::tuple<glm::mat4, glm::mat4, glm::mat4, glm::vec3>;
	private:
		graphics::core::context_base& ctx;
		const graphics::core::vertex_shader&vshader;
		const graphics::core::pixel_shader&pshader;
		std::vector<Mesh> meshes;
		graphics::core::vertex_stream<glm::vec3, glm::vec2> stream;
		
		

		graphics::core::constant_buffer<cb_t> cb;
		glm::mat4 transform;
	public:
		Model(graphics::core::context_base& ctx, std::string_view file, const graphics::core::vertex_shader& vs, const graphics::core::pixel_shader& ps);
		Model(Model&&) = default;
		Model(Model const&) = delete;
		Model& operator=(Model&&) = default;
		Model& operator=(Model const&) = delete;

		inline void set_transform(glm::mat4 tr)
		{
			transform = tr;
		}
		void render(glm::mat4 view, glm::mat4 proj);

		//void render(glm::mat4 const& model, glm::mat4 const& view, glm::mat4 const& proj, gl::program&);

	};
	

}