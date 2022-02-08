#pragma once
#include<utility>

namespace alya
{

	namespace details
	{
		template<typename T>
		struct default_handle_releaser
		{
			void operator()(T)noexcept{}
		};

	}

	template<typename T, typename Releaser = details::default_handle_releaser<T>>
	class unique_handle
	{
	public:

		using value_type = typename T;
		using releaser_type = typename Releaser;

		unique_handle(value_type v, releaser_type&& r) noexcept : value_(v), releaser(std::move(r)) {}
		unique_handle(value_type v, releaser_type const& r) noexcept : value_(v), releaser(r) {}
		unique_handle(value_type v = 0) noexcept : unique_handle(v, releaser_type{}) {}

		unique_handle(unique_handle&& h)noexcept : unique_handle() { operator=(std::move(h)); }
		unique_handle(unique_handle const&) = delete;
		unique_handle& operator=(unique_handle&& h) noexcept { reset(h.release()); releaser = std::move(h.releaser); return *this; }
		unique_handle& operator=(unique_handle const& h) = delete;

		unique_handle& operator=(value_type v) { reset(v); return *this; }

		~unique_handle()noexcept { reset(); }


		value_type release()noexcept
		{
			value_type tmp = value_;
			value_ = 0;
			return tmp;
		}

		void reset(value_type v = 0)noexcept
		{
			if (*this)
				releaser(value_);
			value_ = v;
		}

		void swap(unique_handle&h)noexcept
		{
			std::swap(value_, h.value_);
			std::swap(releaser, h.releaser);
		}

		releaser_type& get_releaser()noexcept
		{
			return releaser;
		}

		const releaser_type& get_releaser()const noexcept
		{
			return releaser;
		}

		value_type value()const noexcept { return value_; }
		operator value_type() const noexcept{ return value(); }
		operator bool() const noexcept{ return value() != 0; }

	private:
		value_type value_;
		releaser_type releaser;
	};


}
