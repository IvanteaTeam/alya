#pragma once
#include<span>
#include<alya/graphics/buffer_base.hpp>

namespace alya::graphics
{

	template<typename T>
	class basic_buffer_map;

	template<typename T, memory_qualifier, buffer_binding>
	class basic_buffer;

	template<typename T>
	class basic_const_buffer_map : protected buffer_map_base
	{
	public:
		using value_type = T;

		using iterator = const value_type*;

		constexpr const value_type* data()const
		{
			return reinterpret_cast<const value_type*>(buffer_map_base::data());
		}

		constexpr size_t size()const
		{
			return buffer_map_base::size() / sizeof(value_type);
		}

		constexpr iterator begin()const
		{
			return data();
		}

		constexpr iterator end()const
		{
			return begin() + size();
		}

	private:
		explicit basic_const_buffer_map(buffer_map_base&&base) : buffer_map_base(std::move(base)) {}

		friend class basic_buffer_map<T>;
		
		template<typename T, buffer_binding B>
		friend basic_const_buffer_map<T> map_read(basic_buffer<T, memory_qualifier::staging, B>&);
		template<typename T, buffer_binding B>
		friend basic_buffer_map<T> map_write(basic_buffer<T, memory_qualifier::staging, B>&);
		template<typename T, buffer_binding B>
		friend basic_buffer_map<T> map_write_discard(basic_buffer<T, memory_qualifier::dynamic, B>&);
	};

	template<typename T>
	class basic_buffer_map : public basic_const_buffer_map<T>
	{
	public:
		using const_buffer_map = basic_const_buffer_map<T>;
		using value_type = T;
		using iterator = value_type*;
		using const_buffer_map::const_buffer_map;
		
		constexpr value_type* data()
		{
			return const_cast<value_type*>(const_buffer_map::data());
		}

		constexpr iterator begin()
		{
			return data();
		}

		constexpr iterator end()
		{
			return begin() + const_buffer_map::size();
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
		basic_buffer(std::span<const value_type>data, Context&ctx)
			: basic_buffer(data.begin(), data.end(), ctx) {}

		template<typename Context>
		basic_buffer(std::contiguous_iterator auto begin, std::contiguous_iterator auto end, Context& ctx)
			: basic_buffer(std::addressof(*begin), end - begin, ctx) {}

		template<typename Context>
		basic_buffer(const value_type* data, size_t count, Context& ctx)
			: buffer_base(data, count, Q, B, ctx) {}

		constexpr size_t size()const
		{
			return buffer_base::size() / sizeof(value_type);
		}

	private:
		/*
		details::buffer_map_base map(details::d3d_map_type type)
		{
			return buffer_base::map(type);
		}*/

		template<typename T, buffer_binding B>
		friend basic_const_buffer_map<T> map_read(basic_buffer<T, memory_qualifier::staging, B>&);
		template<typename T, buffer_binding B>
		friend basic_buffer_map<T> map_write(basic_buffer<T, memory_qualifier::staging, B>&);
		template<typename T, buffer_binding B>
		friend basic_buffer_map<T> map_write_discard(basic_buffer<T, memory_qualifier::dynamic, B>&);
	};

	template<typename T, buffer_binding B>
	basic_const_buffer_map<T> map_read(basic_buffer<T, memory_qualifier::staging, B>&buffer)
	{
		return basic_const_buffer_map<T>{details::map(buffer, details::map_type::read)};
	}

	template<typename T, buffer_binding B>
	basic_buffer_map<T> map_write(basic_buffer<T, memory_qualifier::staging, B>& buffer)
	{
		return basic_buffer_map<T>{details::map(buffer, details::map_type::write)};
	}

	template<typename T, buffer_binding B>
	basic_buffer_map<T> map_write_discard(basic_buffer<T, memory_qualifier::dynamic, B>& buffer)
	{
		return basic_buffer_map<T>{details::map(buffer, details::map_type::write_discard)};
	}
	
}
