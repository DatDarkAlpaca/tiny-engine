#pragma once
#include "keyboard_event.hpp"

namespace tiny
{
	struct KeyReleasedEvent : public Event
	{
		DEFINE_EVENT(EventType::KEY_RELEASED);

	public:
		KeyReleasedEvent(Key key, KeyboardMods mods)
			: key(key)
			, mods(mods)
		{

		}

	public:
		Key key;
		KeyboardMods mods;
	};
}