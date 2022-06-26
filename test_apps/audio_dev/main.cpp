
#include<iostream>
#include<alya/audio/core/wave_format.hpp>
#include<alya/audio/core/basic_system_endpoint.hpp>
#include<alya/audio/core/device_enumerator.hpp>
#include<alya/audio/basic_sample.hpp>
#include<alya/resource/file_loader.hpp>
#include<alya/resource/async_import.hpp>
#include<boost/asio/io_context.hpp>
#include<alya/async/delay.hpp>
#include<alya/audio/basic_context.hpp>
#include<alya/audio/basic_sample_player.hpp>

using namespace alya;

using wf = audio::core::wave_format<float, 2>;

boost::asio::io_context ioc;

async::promise<void> entry()
{
	using namespace std::chrono_literals;
	resource::file_loader loader;

	std::cout << "Enter path: ";

	std::string path;
	std::getline(std::cin, path);
	

	auto sample = co_await resource::async_import<audio::basic_sample<wf>>(loader, path, ioc.get_executor());
	
	audio::core::basic_system_endpoint<wf> ep(audio::core::device_enumerator{}.get_default_device().value(), sample.sample_rate());

	audio::basic_context<wf> ctx(ep);

	audio::basic_sample_player<wf> sp(sample);

	ctx.set_otput_channel(sp);
	sp.play();
	while (sp.is_playing());

}

int main()
{
	using namespace std::chrono_literals;
	using namespace alya;

	std::atomic<bool> end = false;

	entry().then([&]() { end = true; }).except([&](auto e) {
		try
		{
			std::rethrow_exception(e);
			}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}
		end = true;
		});

	while (!end)
		ioc.run();


	return 0;
}
