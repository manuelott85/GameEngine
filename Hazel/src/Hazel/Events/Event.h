#pragma once

#include "Hazel/Core.h"

#include <string>
#include <functional>

namespace Hazel {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

	// static function is needed to be able to get the event type from the "event" base class (polymorpic); see event-dispatcher in this document to see an example
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class HAZEL_API Event	// Base Class for Events
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;	// pure virtual and MUST be implemented
		virtual const char* GetName() const = 0;	// pure virtual and MUST be implemented
		virtual int GetCategoryFlags() const = 0;	// pure virtual and MUST be implemented
		virtual std::string ToString() const { return GetName(); }	// by default: will return the name, but can be overriden to add some additional information (debug-use only)

		inline bool IsInCategory(EventCategory category)	// returns true, if the event is of given category otherwise false
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;	// a flag to determine if you want to be passed any further by a dispatcher (e.g. if you click on a button with the mouse, the button already "consumes" the event and the game underneath does not need to bother any more)
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)& m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
