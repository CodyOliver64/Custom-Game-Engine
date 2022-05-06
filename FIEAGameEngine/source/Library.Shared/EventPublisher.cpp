#include "pch.h"
#include "EventPublisher.h"
#include <assert.h>


namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	
	EventPublisher::EventPublisher(const Vector<EventSubscriber*>& subscriberList)
		: _subscriberList(&subscriberList)
	{
	}

	void EventPublisher::Deliver()
	{
		for (EventSubscriber* subscriber : *_subscriberList)
		{
			subscriber->Notify(*this);
		}
	}
}
