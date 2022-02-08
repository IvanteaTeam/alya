#pragma once
#include<alya/audio/decoder_base.hpp>
#include<alya/audio/core/sample.hpp>

namespace alya::audio
{

	template<core::sample Sample>
	class basic_decoder : public decoder_base
	{
	public:

		using sample_type = Sample;

		basic_decoder() = default;

		template<std::contiguous_iterator I>
		void open(I begin, I end, std::error_code& e)
		{
			decoder_base::open(&*begin, (end - begin) * sizeof(typename std::iterator_traits<I>::value_type), e);
		}

		template<std::contiguous_iterator I> requires std::same_as<typename std::iterator_traits<I>::value_type, sample_type>
		size_t decode_some(I begin, I end, std::error_code&e)noexcept
		{
			return decoder_base::decode_some(&*begin, end - begin, e);
		}

	private:

	};

}
