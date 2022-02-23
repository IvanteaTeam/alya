#pragma once
#include<alya/utility/windows/com/shared_ptr.hpp>

struct ID3D11Device;
struct ID3D11SamplerState;

namespace alya::graphics::core::details
{

	class d3d11_sampler
	{
	public:

		using native_handle_type = windows::com::shared_ptr<ID3D11SamplerState>;

		explicit d3d11_sampler(windows::com::shared_ptr<ID3D11Device>);

		d3d11_sampler(const d3d11_sampler&) = delete;
		d3d11_sampler(d3d11_sampler&&) = default;
		d3d11_sampler& operator=(const d3d11_sampler&) = delete;
		d3d11_sampler& operator=(d3d11_sampler&&) = default;

		native_handle_type native_handle()const noexcept
		{
			return impl_;
		}

	private:
		windows::com::shared_ptr<ID3D11SamplerState> impl_;
	};

}
