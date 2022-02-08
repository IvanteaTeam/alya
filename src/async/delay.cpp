#include<alya/async/delay.hpp>
#include<boost/asio/steady_timer.hpp>

namespace alya::async
{

	promise<std::chrono::milliseconds> delay_for(std::chrono::milliseconds dur)
	{
		auto [p, d] = make_promise<std::chrono::milliseconds>();

		auto timer = std::make_shared<boost::asio::steady_timer>(boost::asio::system_executor{});

		auto start = std::chrono::steady_clock::now();

		timer->expires_after(dur);
		timer->async_wait([d = std::move(d), start, timer](auto e) mutable{
			d.set_value(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start));
		});

		return std::move(p);
	}

}
