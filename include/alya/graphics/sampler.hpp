#pragma once
#include<alya/graphics/details/d3d11/object_base.hpp>

namespace alya
{

	namespace graphics
	{

		class sampler : protected d3d11::object_base
		{
		public:

			template<typename C>
			sampler(C& ctx) : sampler(get_device(ctx), get_device_context(ctx)) {}
			sampler(sampler&&) = default;
			sampler(const sampler&) = delete;
			sampler& operator=(sampler&&) = default;
			sampler& operator=(const sampler&) = delete;



		private:
			sampler(d3d11::device_ptr, d3d11::device_context_ptr);
			mutable d3d11::sampler_state_ptr state;
			friend class context_base;
		};

	}

}
