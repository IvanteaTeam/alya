#include<thread>
#include<iostream>
#include<boost/asio/thread_pool.hpp>
#include<alya/resource/async_import.hpp>
#include<alya/resource/file_loader.hpp>
#include<alya/ui/core/window.hpp>
#include<alya/graphics/context.hpp>
#include<alya/graphics/vertex_buffer.hpp>
#include<alya/graphics/vertex_stream.hpp>
#include<alya/graphics/frame_buffer.hpp>
#include<alya/graphics/color_buffer_view.hpp>
#include<alya/graphics/depth_stencil_buffer_view.hpp>
#include<glm/glm.hpp>
#include<glm/ext.hpp>
#include"model.hpp"
#include"scene.hpp"
#include<alya/resource/image.hpp>
#include<alya/ui/core/get_event.hpp>
#include<alya/ui/core/events.hpp>
using namespace alya;

#define BUILD_DIR __FILE__"/../../../build/bin/"

std::atomic<bool> end = false;

namespace gfx = alya::graphics;

void show_exception(std::exception_ptr e)
{
	try
	{
		std::rethrow_exception(e);
	}
	catch (const std::system_error& e)
	{
		std::cout << e.code() << "\n";
	}
	catch (const std::exception&e)
	{
		std::cout << e.what() << "\n";
	}
}

void f()
{

}

void run()
{
	namespace res = resource;

	res::file_loader loader;

	boost::asio::thread_pool tp(1);
	
	ui::core::window window;
	window.open();
	gfx::basic_context<rgba, gfx::depth_stencil_value<32, 0>, 4> ctx(window);

	std::optional<gfx::vertex_shader> vs, vs1, mvs;
	std::optional<gfx::pixel_shader> ps, ps1, mps;
	res::image image;
	std::atomic<int> shaders_loaded = 0;
	
	res::async_import<gfx::vertex_shader>(loader, BUILD_DIR"a.cso", tp.get_executor(), ctx).then([&](auto s) { vs.emplace(std::move(s)); shaders_loaded++; });
	res::async_import<gfx::pixel_shader>(loader, BUILD_DIR"b.cso", tp.get_executor(), ctx).then([&](auto s) { ps.emplace(std::move(s)); shaders_loaded++; });
	res::async_import<gfx::vertex_shader>(loader, BUILD_DIR"d1.cso", tp.get_executor(), ctx).then([&](auto s) { vs1.emplace(std::move(s)); shaders_loaded++; });
	res::async_import<gfx::pixel_shader>(loader, BUILD_DIR"d2.cso", tp.get_executor(), ctx).then([&](auto s) { ps1.emplace(std::move(s)); shaders_loaded++; });
	res::async_import<gfx::vertex_shader>(loader, BUILD_DIR"m1.cso", tp.get_executor(), ctx).then([&](auto s) { mvs.emplace(std::move(s)); shaders_loaded++; });
	res::async_import<gfx::pixel_shader>(loader, BUILD_DIR"m2.cso", tp.get_executor(), ctx).then([&](auto s) { mps.emplace(std::move(s)); shaders_loaded++; });
	res::async_import<res::image>(loader, __FILE__"/../models/2/textures/default_baseColor.png", tp.get_executor(), res::flip_vertically)
		.then([&](auto i) { image = std::move(i); shaders_loaded++; });

	while (shaders_loaded < 7)
	{
		
	}

	gfx::sampler sampler(ctx);

	auto width = window.client_width();
	auto height = window.client_height();

	dev::Model model(ctx, __FILE__"/../models/1/yes2.fbx", *mvs, *mps);

	dev::Scene scene(ctx, model);

	scene.on_window_resize(width, height);

	window.set_event_callback([&](auto e)noexcept {
		
		if (dynamic_cast<ui::core::should_close*>(e))
			end = true;
		if (dynamic_cast<ui::core::should_minimize*>(e))
			window.minimize();
		if (dynamic_cast<ui::core::should_maximize*>(e))
			window.maximize();
		if (dynamic_cast<ui::core::should_restore*>(e))
			window.restore();
		if (dynamic_cast<ui::core::resized*>(e))
		{
			width = window.client_width();
			height = window.client_height();
			scene.on_window_resize(width, height);
		}
		if (auto mm = dynamic_cast<ui::core::raw_mouse_input*>(e))
		{
			scene.on_mouse_move(mm->x, mm->y);
		}
	});

	gfx::texture2d<rgba> texture_of_model(gfx::generate_mipmaps, image, ctx);

	gfx::texture_view view{ texture_of_model, 0, texture_of_model.mipmaps() };

	while (!end)
	{
		auto begin = std::chrono::high_resolution_clock::now();
		
		while (ui::core::poll_event());

		gfx::depth_stencil_state dss{  };

		ctx.set_depth_stencil_state(dss);
		ctx.set_viewport(0, 0, width, height);

		ctx.clear_color(1, 1, 1, 1);
		ctx.clear_depth(1);

		ctx.bind_texture(gfx::pixel_shader_target, view, 0);
		ctx.bind_frame_buffer(gfx::default_frame_buffer);
		ctx.bind_sampler(gfx::pixel_shader_target, sampler, 0);
		scene.render();

		ctx.present();
		ctx.clear();
		auto end = std::chrono::high_resolution_clock::now();
		
		std::cout << (std::chrono::seconds(1) / (end - begin)) << "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(16) - (end - begin));
	}


}

int main()
{
	
	try {
		run();
	}
	catch (const std::exception&e)
	{
		std::cout << e.what() << "\n";
		throw;
	}

	return 0;
}



