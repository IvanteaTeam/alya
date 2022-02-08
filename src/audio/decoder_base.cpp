#include<alya/audio/decoder_base.hpp>
#include<sndfile.h>

namespace alya::audio
{

	decoder_base::decoder_base()noexcept : 
		file_(nullptr, [](auto file) { if (file) sf_close(file); }),
		frames_(0),
		sample_rate_(0),
		data_(nullptr),
		size_(0),
		channels_(0),
		offset_(0)
	{}

	void decoder_base::open(const void*data, size_t size, std::error_code&e)noexcept
	{
		e = {};
		close();

		data_ = data;
		size_ = size;

		SF_VIRTUAL_IO vio;

		vio.write = nullptr;
		vio.read = [](void* buff, int64_t size, void* ptr) { return static_cast<decoder_base*>(ptr)->read(buff, size); };
		vio.seek = [](int64_t offset, int whence, void* ptr) { return static_cast<decoder_base*>(ptr)->seek(offset, whence); };
		vio.get_filelen = [](void* ptr) { return static_cast<decoder_base*>(ptr)->len(); };
		vio.tell = [](void* ptr) { return static_cast<decoder_base*>(ptr)->tell(); };

		SF_INFO info;

		file_.reset(sf_open_virtual(&vio, SFM_READ, &info, this));

		if (file_)
		{
			sample_rate_ = info.samplerate;
			channels_ = info.channels;
			frames_ = info.frames;
		}
		else
		{
			close();
			e = get_error();
		}

	}

	void decoder_base::close()noexcept
	{
		file_.reset();
		offset_ = 0;
	}

	bool decoder_base::is_open()const noexcept
	{
		return bool(file_);
	}

	size_t decoder_base::frames()const noexcept
	{
		return frames_;
	}

	size_t decoder_base::channels()const noexcept
	{
		return channels_;
	}

	size_t decoder_base::sample_rate()const noexcept
	{
		return sample_rate_;
	}

	std::error_code decoder_base::get_error()const noexcept
	{
		return static_cast<decoder_error>(sf_error(file_.get()));
	}

	int64_t decoder_base::tell()noexcept
	{
		return offset_;
	}
	int64_t decoder_base::seek(int64_t offset, int whence)noexcept
	{
		switch (whence)
		{
		case SEEK_SET:
			offset_ = offset;
			break;

		case SEEK_CUR:
			offset_ += offset;
			break;

		case SEEK_END:
			offset_ = size_ + offset;
			break;
		default:
			break;
		};

		return offset_;
	}
	int64_t decoder_base::read(void* buff, int64_t size)noexcept
	{
		int64_t len = std::min(size, size_ - offset_);

		memcpy(buff, static_cast<const uint8_t*>(data_) + offset_, len);
		offset_ += len;
		return len;
	}
	int64_t decoder_base::len()noexcept
	{
		return size_;
	}

	size_t decoder_base::decode_some(int16n_t* ptr, size_t samples, std::error_code& e)noexcept
	{
		size_t readed = sf_read_short(file_.get(), reinterpret_cast<short*>(ptr), samples);
		e = get_error();
		return readed;
	}

	size_t decoder_base::decode_some(int32n_t* ptr, size_t samples, std::error_code& e)noexcept
	{
		size_t readed = sf_read_int(file_.get(), reinterpret_cast<int*>(ptr), samples);
		e = get_error();
		return readed;
	}

	size_t decoder_base::decode_some(float* ptr, size_t samples, std::error_code& e)noexcept
	{
		size_t readed = sf_read_float(file_.get(), ptr, samples);
		e = get_error();
		return readed;
	}

}
