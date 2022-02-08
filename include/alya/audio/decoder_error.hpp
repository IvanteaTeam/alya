#pragma once
#include<system_error>

namespace alya::audio
{

	enum class decoder_error
	{
		unrecognised_format = 1,
		malformed = 3,
		unsupported_encoding = 4
	};

	const std::error_category& decoder_error_category()noexcept;

	inline std::error_code make_error_code(decoder_error err)
	{
		return std::error_code{static_cast<int>(err), decoder_error_category()};
	}

}

namespace std
{

	template<>
	struct is_error_code_enum<alya::audio::decoder_error> : std::true_type {};

}
