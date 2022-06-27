#pragma once
#include<alya/async/promise.hpp>
#include<string>
#include<vector>

namespace alya::resource
{
	
	class file_reader
	{
	public:
		using data_type = std::vector<char>;
		using source_type = std::string;
		async::promise<data_type> async_read(source_type path);
	};

}
