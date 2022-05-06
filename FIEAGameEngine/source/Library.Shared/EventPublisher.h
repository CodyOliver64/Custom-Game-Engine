#pragma once

#include "RTTI.h"
#include "Vector.h"
#include "EventSubscriber.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Abstract base class for events. Used to implement the Deliver method that is common between all families of events
	/// </summary>
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)

	public:
		EventPublisher() = delete;

		/// <summary>
		/// Deliver method that loops through the list of subscribers and delivers the Event to each subscriber in the list
		/// </summary>
		void Deliver();

	protected:
		EventPublisher(const Vector<EventSubscriber*>& subscriberList);

		EventPublisher(const EventPublisher&) = default;
		EventPublisher& operator=(const EventPublisher&) = default;
		EventPublisher(EventPublisher&&) noexcept = default;
		EventPublisher& operator=(EventPublisher&&) noexcept = default;
		virtual ~EventPublisher() = default;

		const Vector<EventSubscriber*>* _subscriberList;
	};
}


