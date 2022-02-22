#pragma once
#include<alya/graphics/core/attribute_stream.hpp>
#include<alya/graphics/core/details/pixel_type.hpp>
#include<string>
#include<vector>

namespace alya::graphics::core
{

	struct attribute_semantic
	{
		std::string_view name;
		size_t index;
	};

	class vertex_stream_base : public d3d11::object_base
	{
	public:

		template<typename S, typename C>
		vertex_stream_base(const std::vector<std::pair<attribute_semantic, details::pixel_type>>& sem, const S& shader, C& ctx)
			: vertex_stream_base(sem, shader.bytecode, get_device(ctx), get_device_context(ctx)) {}

	protected:

		void set(size_t i, const attribute_stream_base& a);
		vertex_stream_base(const std::vector<std::pair<attribute_semantic, details::pixel_type>>&, const std::vector<char>& bytecode, d3d11::device_ptr, d3d11::device_context_ptr);
	private:

		void bind()const;

		mutable d3d11::input_layout_ptr layout;
		std::vector<attribute_stream_base> attributes;
		friend class context_base;
	};

}
