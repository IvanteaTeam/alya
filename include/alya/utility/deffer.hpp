#pragma once
#include<functional>

namespace alya
{
	class deffer
	{
	public:

		template<typename F>
		constexpr deffer(F&&f) : func(std::forward<F>(f)) {}
		inline ~deffer()
		{
			func();
		}
		deffer(deffer&&) = delete;
		deffer(const deffer&) = delete;
		deffer& operator=(deffer&&) = delete;
		deffer& operator=(const deffer&) = delete;

	private:
		std::function<void()> func;
	};
}
