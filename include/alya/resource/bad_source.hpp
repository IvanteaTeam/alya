#include<stdexcept>

namespace alya::resource
{
	
	class bad_source : public std::runtime_error
	{
	public:
		explicit bad_source(std::string msg) : runtime_error(std::move(msg)) {}
	};

}
