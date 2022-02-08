#include<iostream>
#include<alya/ui/core/window.hpp>
#include<alya/ui/core/get_event.hpp>
#include<thread>
#include<alya/ui/core/events.hpp>

int main()
{
	
	using namespace alya;
	using namespace std::chrono_literals;

	ui::core::window window;


	window.set_event_callback([&](auto e) {
		
		
		if (dynamic_cast<ui::core::focus_event*>(e))
		{
			if (window.has_focus())
				std::cout << "got focus\n";
			else
				std::cout << "lose focus\n";
		}
		if (dynamic_cast<ui::core::should_close*>(e))
			window.close();
		if (dynamic_cast<ui::core::should_minimize*>(e))
			window.minimize();
		if (dynamic_cast<ui::core::should_maximize*>(e))
			window.maximize();
		if (dynamic_cast<ui::core::should_restore*>(e))
			window.restore();

		if (auto ke = dynamic_cast<ui::core::key_event*>(e); ke)
		{
			if (dynamic_cast<ui::core::key_down*>(ke))
				std::cout << "Key down: " << ke->key.name() << "\n";
			else if (dynamic_cast<ui::core::key_up*>(ke))
				std::cout << "Key up: " << ke->key.name() << "\n";
		}

		if (auto me = dynamic_cast<ui::core::mouse_event*>(e); me)
		{
			if (dynamic_cast<ui::core::mouse_down*>(me))
				std::cout << "Mouse down at" << me->x << " " << me->y << "\n";
		}

		if (dynamic_cast<ui::core::resized*>(e))
		{
			std::cout << "resized\n";
		}

	});
	
	window.open();

	while (window.is_open())
	{
		while(ui::core::poll_event());
	}

	
	return 0;
}
