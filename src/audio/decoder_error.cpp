#include<alya/audio/decoder_error.hpp>
#include<sndfile.h>

namespace alya::audio
{

	class decoder_category : public std::error_category
	{
	public:
		virtual const char* name()const noexcept override
		{
			return "decoder error";
		}
		virtual std::string message(int e)const noexcept override
		{
			return sf_error_number(e);
		}
	private:
		decoder_category() = default;
		friend const std::error_category& decoder_error_category()noexcept;
	};

	const std::error_category& decoder_error_category()noexcept
	{
		static decoder_category ec;
		return ec;
	}

}
