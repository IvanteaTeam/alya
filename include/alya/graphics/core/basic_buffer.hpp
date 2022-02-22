#pragma once
#include<span>
#include<alya/graphics/core/buffer_base.hpp>
#include<alya/graphics/core/map.hpp>

namespace alya::graphics::core
{

	template<typename T>
	class buffer_map;

	template<typename T, memory_qualifier, buffer_binding>
	class basic_buffer;

	template<typename T>
	class buffer_const_map : public buffer_map_base
	{
	public:
		using value_type = T;

		using iterator = const value_type*;

		const value_type* data()const noexcept
		{
			return static_cast<const value_type*>(buffer_map_base::data());
		}

		size_t size()const noexcept
		{
			return buffer_map_base::size() / sizeof(value_type);
		}

		iterator begin()const noexcept
		{
			return data();
		}

		iterator end()const noexcept
		{
			return begin() + size();
		}

	private:
		buffer_const_map(buffer_map_base&&base) : buffer_map_base(std::move(base)) {}

		friend class buffer_map<value_type>;
		template<typename, memory_qualifier, buffer_binding>
		friend class basic_buffer;
	};

	template<typename T>
	class buffer_map : public buffer_const_map<T>
	{
	public:
		
		using value_type = T;
		using iterator = value_type*;
		using buffer_const_map<value_type>::buffer_const_map;
		
		value_type* data()noexcept
		{
			return const_cast<value_type*>(buffer_const_map<value_type>::data());
		}

		iterator begin()noexcept
		{
			return data();
		}

		iterator end()noexcept
		{
			return begin() + buffer_const_map<value_type>::size();
		}

	};

	template<typename T, memory_qualifier Q, buffer_binding B>
	class basic_buffer : public buffer_base
	{
	public:

		using value_type = T;
		
		template<typename Context>
		basic_buffer(size_t count, Context& ctx)
			: basic_buffer(nullptr, count, ctx) {}

		template<typename Context>
		basic_buffer(std::contiguous_iterator auto begin, std::contiguous_iterator auto end, Context& ctx)
			: basic_buffer(&*begin, end - begin, ctx) {}

		template<typename Context>
		basic_buffer(const value_type* data, size_t count, Context& ctx)
			: buffer_base(data, count * sizeof(value_type), Q, B, ctx) {}

		size_t size()const noexcept
		{
			return buffer_base::size() / sizeof(value_type);
		}

		buffer_const_map<value_type> map(map_t::read_t)
		{
			return buffer_base::map(details::map_type::read);
		}

		buffer_map<value_type> map(map_t::write_t)
		{
			return buffer_base::map(details::map_type::write);
		}

		buffer_map<value_type> map(map_t::read_write_t)
		{
			return buffer_base::map(details::map_type::read_write);
		}

		buffer_map<value_type> map(map_t::write_discard_t)
		{
			return buffer_base::map(details::map_type::write_discard);
		}
		
	};
	
}
