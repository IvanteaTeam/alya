#pragma once

namespace alya::graphics::core
{

	constexpr struct map_t
	{
		static constexpr struct write_t {} write{};
		static constexpr struct read_t {} read{};
		static constexpr struct read_write_t {} read_write{};
		static constexpr struct write_discard_t {} write_discard{};
	} map;

}
