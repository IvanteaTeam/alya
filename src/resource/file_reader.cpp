#include<alya/resource/file_reader.hpp>
#include<boost/asio/stream_file.hpp>
#include<boost/asio/read.hpp>
#include<boost/asio/streambuf.hpp>
#include<alya/resource/bad_source.hpp>

namespace alya::resource
{
	
	async::promise<std::vector<char>> file_reader::async_read(std::string path)
	{
		
		struct ctx_t
		{
			ctx_t() : buffer(), file(boost::asio::require(boost::asio::system_executor{}, boost::asio::execution::blocking.never)) {}

			boost::asio::streambuf buffer;
			boost::asio::stream_file file;
		};

		auto ctx = std::make_shared<ctx_t>();
		
		boost::system::error_code e;

		ctx->file.open(path, boost::asio::file_base::flags::read_only, e);
		
		if (e)
			throw bad_source(path + " not found.");

		auto [p, d] = async::make_promise<std::vector<char>>();

		boost::asio::async_read(ctx->file, ctx->buffer, [d = std::move(d), ctx, path](boost::system::error_code e, size_t size)mutable {
		
			if (e && e != boost::asio::error::misc_errors::eof)
			{
				d.set_exception(bad_source{path + " not found."});
			}
			else
			{
				try {
					std::vector<char> data(size);
					auto it = data.begin();
					for (auto buffer : ctx->buffer.data())
					{
						auto ptr = static_cast<const char*>(buffer.data());
						std::copy(ptr, ptr + buffer.size(), it);
						it += buffer.size();
					}
					d.set_value(std::move(data));
				}
				catch (...)
				{
					d.set_exception(std::current_exception());
				}
			}
		});

		return std::move(p);
	}

}
