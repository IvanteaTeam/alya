#pragma once
#include<alya/ui/core/key.hpp>

#include<chrono>

#include "model.hpp"

using namespace alya;

namespace dev {
	
	class Scene {
	private:


		graphics::core::context_base& ctx;
		Model&model;
		
		glm::vec2 angle{ 0, 0 }, pos{ 0, 0 }, window{1000, 1000};
		glm::vec2 vel{ 0, 0 };

		glm::mat4 model_tr;
		float model_rot = 0;

	public:

		static std::string root;

		void on_window_resize(int w, int h)
		{
			window.x = w;
			window.y = h;
		}

		void on_mouse_move(int x, int y)
		{
			angle += 0.003f * glm::vec2(x, y);
			angle.y = std::clamp(angle.y, -3.14f / 2.0f, 3.14f / 2.0f);
		}

		void update()
		{
			using alya::ui::core::key;
			static auto prev_frame = std::chrono::high_resolution_clock::now();

			auto now = std::chrono::high_resolution_clock::now();

			auto dt = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - prev_frame).count();

			prev_frame = now;

			auto step = [&](glm::vec2 dir)
			{
				auto res = glm::vec4(dir.y, 0, -dir.x, 0) * glm::rotate(glm::mat4(1), angle.x, glm::vec3(0, 1, 0));
				return glm::vec2(res.x, res.z);
			};

			glm::vec2 dir{ 0, 0 };

			if (key(key::A).state() || key(key::arrow_left).state())
				dir += glm::vec2(0, -1);
			if (key(key::W).state() || key(key::arrow_up).state())
				dir += glm::vec2(1, 0);
			if (key(key::D).state() || key(key::arrow_right).state())
				dir += glm::vec2(0, 1);
			if (key(key::S).state() || key(key::arrow_down).state())
				dir += glm::vec2(-1, 0);

			if (dir.x || dir.y)
				vel += step(glm::normalize(dir)) * dt * 0.001f * (key(key::space).state() ? 4.0f : 1.0f);
			pos += vel;

			//if (glm::length(vel) < 1)
			//	vel.x = vel.y = 0;

			auto frc = -glm::normalize(vel) * 0.00001f;

			vel *= 0.5;

		//	vel += frc;

			if (key(key::escape).state())
				::end = true;

			model_tr = glm::rotate(glm::mat4(1), model_rot, glm::vec3(0, 1, 0));
			//model_rot += dt * 0.001;

		}

		inline Scene(graphics::core::context_base&ctx, Model&model) : 
			ctx(ctx),
			//model1(ctx, root + "models/1/yes1.fbx"),
			//model2(ctx, root + "models/2/2.fbx"),
			//shader1(help::load_shader(ctx, root + "shaders/1/v.glsl", root + "shaders/1/f.glsl")),
			//shader2(help::load_shader(ctx, root + "shaders/2/v.glsl", root + "shaders/2/f.glsl")),
			//color_texture(help::load_texture(ctx, root + "models/2/textures/default_baseColor.png"))
			model(model)
		{

		}

		inline void render() 
		{
			update();

			glm::mat4 view = glm::mat4(1), transform = glm::scale(glm::mat4(1), glm::vec3(0.02, 0.02, 0.02)),
				proj = glm::perspectiveRH(glm::radians(60.0f), float(window.x) / float(window.y), 0.1f, 100.0f);

			//std::cout << ax << " " << ay << "\n";

			//float ax = std::asin((float)(float(mx) / float(window_w / 2)));
			//float ay = -std::asin((float)(float(my) / float(window_h / 2)));

			//std::cout << ax << " " << ay << "\n";

			view = glm::rotate(view, angle.y, glm::vec3(1, 0, 0));
			view = glm::rotate(view, angle.x, glm::vec3(0, 1, 0));
			

			view = glm::translate(view, -glm::vec3(pos.x, 0, pos.y));

			//view = glm::translate(view, glm::vec3(0, -4, -10));
			//view = glm::rotate(view, glm::radians(45.0f), glm::vec3(1, 0, 0));
			//view = glm::rotate(view, timer, glm::vec3(0, 1, 0));
			model.set_transform(transform * model_tr);
			model.render(view, proj);

			//transform = glm::scale(transform, glm::vec3{ 0.01, 0.01, 0.01 });
		}

	};


}