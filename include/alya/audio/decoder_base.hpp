#pragma once
#include<alya/audio/decoder_error.hpp>
#include<alya/utility/normalized.hpp>
#include<memory>

struct SNDFILE_tag;

namespace alya::audio
{

	class decoder_base
	{
	public:

		decoder_base()noexcept;
		decoder_base(const decoder_base&) = delete;
		decoder_base(decoder_base&&) = delete;
		decoder_base& operator=(const decoder_base&) = delete;
		decoder_base& operator=(decoder_base&&) = delete;

		void open(const void*data, size_t size, std::error_code&e)noexcept;
		void close()noexcept;
		bool is_open()const noexcept;

		size_t frames()const noexcept;
		size_t sample_rate()const noexcept;
		size_t channels()const noexcept;

	protected:

		size_t decode_some(int16n_t*, size_t samples, std::error_code&e)noexcept;
		size_t decode_some(int32n_t*, size_t samples, std::error_code& e)noexcept;
		size_t decode_some(float*, size_t samples, std::error_code& e)noexcept;

	private:

		int64_t tell()noexcept;
		int64_t seek(int64_t offset, int whence)noexcept;
		int64_t read(void* buff, int64_t size)noexcept;
		int64_t len()noexcept;

		std::error_code get_error()const noexcept;
		const void* data_;
		int64_t size_, frames_, sample_rate_, channels_, offset_;
		std::unique_ptr<SNDFILE_tag, void(*)(SNDFILE_tag*)> file_;
	};

}
