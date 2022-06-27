#pragma once
#include<alya/async/promise.hpp>
#include<string>
#include<vector>

namespace alya::resource
{
	
	class file_reader
	{
	public:
		using allocator_type = std::allocator<char>;
		using data_type = std::vector<char, allocator_type>;
		using source_type = std::string;
		async::promise<data_type> async_read(source_type path);
	};

}
