#pragma once
#include "mouse_event.hpp"

namespace tiny
{
	struct MouseReleasedEvent : public Event
	{
		DEFINE_EVENT(EventType::MOUSE_RELEASED);

	public:
		MouseReleasedEvent(MouseButton button)
			: button(button)
		{

		}

	public:
		MouseButton button;
	};
}