#pragma once
#include<alya/graphics/details/d3d11/d3d.hpp>

namespace alya::graphics::d3d11
{

	class object_base
	{
	public:

		object_base(device_ptr device, device_context_ptr ctx) : device(device), ctx(ctx) {}
		object_base() = delete;
		object_base(object_base&&) = default;
		object_base& operator=(object_base&&) = default;
		object_base(const object_base&) = delete;
		object_base& operator=(const object_base&) = delete;
	protected:
		template<typename Context>
		static device_ptr get_device(Context& ctx)
		{
			return ctx.device;
		}
		template<typename Context>
		static device_context_ptr get_device_context(Context& ctx)
		{
			return ctx.device_context;
		}

		inline device_ptr get_device()const
		{
			return device;
		}
		device_context_ptr get_device_context()const
		{
			return ctx;
		}

	private:
		mutable device_ptr device;
		mutable device_context_ptr ctx;
	};

}
