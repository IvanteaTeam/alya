#pragma once
#include<stdexcept>

namespace alya::resource
{

	class bad_format : public std::runtime_error
	{
	public:
		explicit bad_format() : bad_format("Invalid format.") {}
		explicit bad_format(std::string msg) : runtime_error(std::move(msg)) {}

	};

}
