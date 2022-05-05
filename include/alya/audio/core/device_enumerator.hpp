#pragma once
#include<alya/audio/core/device.hpp>
#include<alya/audio/core/device_list_events.hpp>
#include<functional>
#include<optional>
#include<alya/audio/core/details/impl/wasapi_device_enumerator.hpp>
#include<algorithm>
#include<iterator>

namespace alya::audio::core
{ 
	/*
	namespace details
	{

		class notification_client;

	}*/

	class device_enumerator
	{
	public:
		
		std::optional<device> get_default_device()const
		{
			auto device_ = impl_.get_default_device();
			if (device_)
				return device(std::move(*device_));
			else
				return std::nullopt;
		}

		std::vector<device> get_devices()const
		{
			auto devices = impl_.get_devices();
			std::vector<device> out;
			out.reserve(devices.size());
			std::transform(devices.begin(), devices.end(), std::back_inserter(out), [](auto&& d) { return device(std::move(d)); });
			return out;
		}

		void set_event_callback(std::function<void()>callback, default_device_changed_t)
		{
			using enum details::device_list_event_type;
			impl_.set_event_callback(callback, default_device_changed);
		}

		void set_event_callback(std::function<void()>callback, device_list_changed_t)
		{
			using enum details::device_list_event_type;
			impl_.set_event_callback(callback, device_list_changed);
		}

	private:
		details::wasapi_device_enumerator impl_;
	};
	
}
