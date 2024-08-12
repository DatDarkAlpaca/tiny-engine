#pragma once
#include "keyboard_event.hpp"

namespace tiny
{
	struct KeyPressedEvent : public Event
	{
		DEFINE_EVENT(EventType::KEY_PRESSED);

	public:
		KeyPressedEvent(Key key, KeyboardMods mods)
			: key(key)
			, mods(mods)
		{

		}

	public:
		Key key;
		KeyboardMods mods;
	};
}