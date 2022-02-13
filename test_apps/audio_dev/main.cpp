#include<iostream>
#include<alya/audio/engine.hpp>
#include<alya/audio/basic_sample.hpp>
#include<alya/resource/file_loader.hpp>
#include<alya/resource/async_import.hpp>
#include<boost/asio/thread_pool.hpp>

using namespace alya;

void show_exception(std::exception_ptr e)
{
	try
	{
		std::rethrow_exception(e);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

void run()
{
	using namespace alya;
	using namespace std::chrono_literals;

	resource::file_loader loader;
	boost::asio::thread_pool tp;
	audio::engine engine;

	std::atomic<bool> end = false;
	
	([&]()->async::promise<void> {

		auto sample = co_await resource::async_import<audio::basic_sample<audio::engine::wave_format>>(loader, __FILE__"/../1.ogg", tp.get_executor());

		co_await audio::async_play(engine, sample);
		
	}
	)().except(show_exception).then([&]() { end = true; });

	while (!end)
		std::this_thread::yield();
}
//#include<Windows.h>

int main()
{
	//SetConsoleOutputCP(CP_UTF8);

	try {

		
		run();

	}
	catch (const std::exception&e)
	{
		std::cout << e.what();
		throw;
	}

	return 0;
}
