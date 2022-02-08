
//#include<alya/gl/details/gl.hpp>

#include"model.hpp"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace dev
{


	static glm::vec4 convert_vector(const aiColor4D& c)
	{
		return glm::vec4(c.r, c.g, c.b, c.r);
	}

	static glm::vec3 convert_vector(const aiColor3D& c)
	{
		return glm::vec3(c.r, c.g, c.b);
	}

	static glm::mat4 convert_matrix(const aiMatrix4x4* from)
	{
		glm::mat4 to;


		to[0][0] = (float)from->a1; to[0][1] = (float)from->b1;  to[0][2] = (float)from->c1; to[0][3] = (float)from->d1;
		to[1][0] = (float)from->a2; to[1][1] = (float)from->b2;  to[1][2] = (float)from->c2; to[1][3] = (float)from->d2;
		to[2][0] = (float)from->a3; to[2][1] = (float)from->b3;  to[2][2] = (float)from->c3; to[2][3] = (float)from->d3;
		to[3][0] = (float)from->a4; to[3][1] = (float)from->b4;  to[3][2] = (float)from->c4; to[3][3] = (float)from->d4;

		return to;
	}


	Mesh::Mesh(graphics::core::context_base&ctx, std::vector<glm::vec3> const&vs, std::vector<glm::vec2> const& tcs)
		: ctx(ctx),
		positions(vs.data(), vs.size(), ctx),
		tex_coords(tcs.data(), tcs.size(), ctx)
	{}

	void Mesh::render()
	{
		/*
		vao.bind();
		ctx.draw(vao, 0, this->vertices_count);
		*/
	}

	static glm::vec3 LoadMaterial(const aiMaterial*mat)
	{
		aiColor4D diffuse_color, ambient, specular;

		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);


		return (convert_vector(diffuse_color) + convert_vector(specular) + convert_vector(ambient));
	}

	static Mesh LoadMesh(graphics::core::context_base&ctx, const aiMesh*mesh)
	{
		std::vector<glm::vec3> vs;
		std::vector<glm::vec2> tcs;

		vs.reserve(mesh->mNumFaces * 3);
		tcs.reserve(mesh->mNumFaces * 3);

		for (int j = 0; j < mesh->mNumFaces; j++)
		{
			auto& face = mesh->mFaces[j];
			auto v1 = mesh->mVertices[face.mIndices[0]];
			auto v2 = mesh->mVertices[face.mIndices[1]];
			auto v3 = mesh->mVertices[face.mIndices[2]];

			vs.push_back({ v1.x, v1.y, v1.z });
			vs.push_back({ v2.x, v2.y, v2.z });
			vs.push_back({ v3.x, v3.y, v3.z });


			if (mesh->mTextureCoords[0] == nullptr)
				continue;

			auto tc1 = mesh->mTextureCoords[0][face.mIndices[0]];
			auto tc2 = mesh->mTextureCoords[0][face.mIndices[1]];
			auto tc3 = mesh->mTextureCoords[0][face.mIndices[2]];
			tcs.push_back({ tc1.x, tc1.y });
			tcs.push_back({ tc2.x, tc2.y });
			tcs.push_back({ tc3.x, tc3.y });
		}

		auto out = Mesh(ctx, vs, tcs);
		return out;
	}

	static void LoadNode(graphics::core::context_base&ctx, std::vector<Mesh>&meshes,const aiScene*scene, const aiNode*node, glm::mat4 parent_transform)
	{
		auto current_transform = parent_transform * convert_matrix(&node->mTransformation);

		for (int i = 0; i < node->mNumMeshes; i++)
		{
			auto* mesh = scene->mMeshes[node->mMeshes[i]];

			auto* material = scene->mMaterials[mesh->mMaterialIndex];

			auto&m = meshes.emplace_back(LoadMesh(ctx, mesh));
			
			m.set_transform(current_transform);
			m.set_color(LoadMaterial(material));

		}

		for (int i = 0; i < node->mNumChildren; i++)
		{
			LoadNode(ctx, meshes, scene, node->mChildren[i], current_transform);
		}
	}

	Model::Model(graphics::core::context_base& ctx, std::string_view file, const graphics::core::vertex_shader& vs, const graphics::core::pixel_shader& ps)
		: ctx(ctx), vshader(vs), pshader(ps), stream({{{"POSITION"}, {"TEX_COORD"}}}, vshader, ctx),
		cb(ctx)
	{
		Assimp::Importer importer;

		auto* scene = importer.ReadFile(file.data(), aiProcess_Triangulate);

		assert(scene);

		LoadNode(ctx, meshes, scene, scene->mRootNode, glm::mat4{ 1.0f });
	}

	void Model::render(glm::mat4 view, glm::mat4 proj)
	{
		ctx.bind_constants(gfx::vertex_shader_target, cb, 0);
		ctx.bind_vertex_shader(vshader);
		ctx.bind_pixel_shader(pshader);
		ctx.bind_vertices(stream);
		
		for (auto& m : meshes)
		{


			auto color = m.get_color();
			cb.emplace(cb_t{ transform * m.get_transform(), view, proj, color });
			
			//m.render();
			stream.attach<0>(m.positions);
			stream.attach<1>(m.tex_coords);
			ctx.draw(0, m.positions.size());
		}

	}

	/*
	void Model::render(glm::mat4 const&transform, glm::mat4 const&view, glm::mat4 const&proj, gl::program&shader)
	{
		shader.use();

		auto ucolor = shader.get_uniform_location("color");
		auto uproj = shader.get_uniform_location("Proj");
		auto uview = shader.get_uniform_location("View");
		auto umodel = shader.get_uniform_location("Model");

		for (auto& m : meshes)
		{


			auto color = m.get_color();
			ucolor.set(m.get_color());
			uproj.set(proj);
			uview.set(view);
			umodel.set(transform * m.get_transform());

			m.render();

		}

	}
	*/

}
