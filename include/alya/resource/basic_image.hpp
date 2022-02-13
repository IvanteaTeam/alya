#pragma once
#include<alya/utility/pixel_traits.hpp>
#include<alya/resource/async_load.hpp>
#include<alya/async/execute.hpp>
#include<alya/resource/bad_format.hpp>
#include<alya/resource/details/image_import_impl.hpp>

namespace alya::resource
{
	
	template<typename Pixel, typename Allocator = std::allocator<typename pixel_traits<Pixel>::element_type>>
	class basic_image
	{
	public:
		using value_type = Pixel;
		using allocator_type = Allocator;

		basic_image(
			const allocator_type& alloc = allocator_type{}
		) noexcept(std::is_nothrow_copy_constructible_v<allocator_type>) : 
			width_(0),
			height_(0),
			row_alignment_(0), 
			alloc_(alloc),
			data_(nullptr)
		{}
		
		basic_image(
			size_t width,
			size_t height,
			size_t row_alignment = 0,
			const allocator_type& alloc = allocator_type{}
		) :
			width_(width),
			height_(height),
			row_alignment_(row_alignment == 0 ? width_ * sizeof(typename allocator_type::value_type) : row_alignment),
			alloc_(alloc),
			data_(alloc_.allocate(alloc_size()))
		{}

		basic_image(const basic_image&) = delete;
		basic_image(basic_image&& other) noexcept(std::is_nothrow_copy_constructible_v<allocator_type>) : 
			width_(other.width_), 
			height_(other.height_), 
			row_alignment_(other.row_alignment_),
			alloc_(other.alloc_),
			data_(other.data_)
		{
			other.data_ = nullptr;
		}

		basic_image& operator=(const basic_image&) = delete;
		basic_image& operator=(basic_image&&other) noexcept(std::is_nothrow_copy_assignable_v<allocator_type>)
		{
			clear();
			width_ = other.width_;
			height_ = other.height_;
			row_alignment_ = other.row_alignment_;
			alloc_ = other.alloc_;
			data_ = other.data_;
			other.data_ = nullptr;
			return *this;
		}

		~basic_image()
		{
			clear();
		}

		constexpr void clear()noexcept
		{
			if (data_)
				alloc_.deallocate(data_, alloc_size());
			width_ = 0;
			height_ = 0;
			row_alignment_ = 0;
			data_ = nullptr;
		}

		constexpr size_t width()const noexcept
		{
			return width_;
		}

		constexpr size_t height()const noexcept
		{
			return height_;
		}

		constexpr size_t row_alignment()const noexcept
		{
			return row_alignment_;
		}

		constexpr value_type* data()noexcept
		{
			return reinterpret_cast<value_type*>(data_);
		}

		constexpr const value_type* data()const noexcept
		{
			return reinterpret_cast<const value_type*>(data_);
		}

		class importer;

	private:
		
		constexpr size_t alloc_size()
		{
			return row_alignment() * height() / sizeof(typename allocator_type::value_type);
		}

		basic_image(
			typename allocator_type::value_type* data, 
			size_t width,
			size_t height,
			size_t row_alignment,
			const allocator_type& alloc
		) noexcept(std::is_nothrow_copy_assignable_v<allocator_type>) :
			width_(width),
			height_(height),
			row_alignment_(row_alignment),
			alloc_(alloc),
			data_(data)
		{}

		size_t width_;
		size_t height_;
		size_t row_alignment_;
		allocator_type alloc_;
		typename allocator_type::value_type* data_;
	};

	struct flip_vertically_t {};
	constexpr flip_vertically_t flip_vertically{};

	template<typename Pixel, typename Allocator>
	class basic_image<Pixel, Allocator>::importer
	{
	public:
		using image_type = basic_image<Pixel, Allocator>;
		using value_type = typename image_type::value_type;
		using allocator_type = typename image_type::allocator_type;

		static_assert(std::is_same_v<typename pixel_traits<value_type>::element_type, uint8n_t>);
		
		auto operator()(const auto&data, const allocator_type& alloc = allocator_type{})
		{
			return do_import(data, alloc, false);
		}

		auto operator()(const auto&data, flip_vertically_t, const allocator_type& alloc = allocator_type{})
		{
			return do_import(data, alloc, true);
		}

	private:

		auto do_import(const auto& data, allocator_type alloc, bool flipped)
		{
			auto data_begin = reinterpret_cast<const char*>(&*std::begin(data));
			auto data_end = reinterpret_cast<const char*>(&*(--std::end(data)));

			auto [image, width, height] = details::image_import_impl<allocator_type>(data_begin, data_end - data_begin, alloc, 4, flipped);

			if (!image)
				throw bad_format{};

			return image_type{image, width, height, width * sizeof(value_type), alloc};
		}

	};

}
