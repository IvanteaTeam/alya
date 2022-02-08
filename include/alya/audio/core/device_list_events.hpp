#pragma once

namespace alya::audio::core
{

	struct device_list_changed_t {};
	struct default_device_changed_t {};

	constexpr device_list_changed_t device_list_changed{};
	constexpr default_device_changed_t default_device_changed{};

}
