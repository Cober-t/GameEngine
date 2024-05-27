#ifndef COLLISION_EVENTS_H
#define COLLISION_EVENTS_H

#include "Event.h"
#include <sstream>

namespace Cober {

	class CollisionBeginEvent : public Event
	{
	public:
		CollisionBeginEvent() = default;
		
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "CollisionBeginEvent!!";
			return ss.str();
		}

		EVENT_CLASS_TYPE(CollisionBegin)
		EVENT_CLASS_CATEGORY(EventCategoryPhysics)
	};


    // class CollisionEndEvent : public Event
	// {
	// public:
	// 	CollisionEndEvent() = default;

	// 	std::string ToString() const override
	// 	{
	// 		std::stringstream ss;
	// 		ss << "CollisionEndEvent!!";
	// 		return ss.str();
	// 	}

	// 	EVENT_CLASS_TYPE(CollisionEnd)
	// 	EVENT_CLASS_CATEGORY(EventCategoryPhysics)
	// };


    // class TriggerEnterEvent : public Event
	// {
	// public:
	// 	TriggerEnterEvent() = default;

	// 	std::string ToString() const override
	// 	{
	// 		std::stringstream ss;
	// 		ss << "TriggerEnterEvent: ";
	// 		return ss.str();
	// 	}

	// 	EVENT_CLASS_TYPE(TriggerEnter)
	// 	EVENT_CLASS_CATEGORY(EventCategoryPhysics)
	// };


    // class TriggerExitEvent : public Event
	// {
	// public:
	// 	TriggerExitEvent() = default;

	// 	std::string ToString() const override
	// 	{
	// 		std::stringstream ss;
	// 		ss << "TriggerExitEvent: ";
	// 		return ss.str();
	// 	}

	// 	EVENT_CLASS_TYPE(TriggerExit)
	// 	EVENT_CLASS_CATEGORY(EventCategoryPhysics)
	// };
}

#endif