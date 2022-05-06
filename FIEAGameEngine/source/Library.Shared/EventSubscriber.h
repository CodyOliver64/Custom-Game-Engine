#pragma once
//#include "EventPublisher.h"

namespace FIEAGameEngine
{
	class EventPublisher;
	
	/// <summary>
	/// Abstract base class for Event subscribers. All objects that inherit from EventSubscriber must implement the Notify method where they can add specific functionality to the delivery of an event.
	/// After implementing Notify, an object can then be a Subscriber in the Event system
	/// </summary>
	class EventSubscriber
	{
	public:
		EventSubscriber(const EventSubscriber&) = default;
		EventSubscriber& operator=(const EventSubscriber&) = default;
		EventSubscriber(EventSubscriber&&) noexcept = default;
		EventSubscriber& operator=(EventSubscriber&&) noexcept = default;
		virtual ~EventSubscriber() = default;

		/// <summary>
		/// Pure Virtual method that subscribers must define. This method will be called on each subscriber of an event type to allow them custom functionality on the delivery of an event
		/// </summary>
		/// <param name="event">Event that was triggered</param>
		virtual void Notify(const EventPublisher& event) = 0;

	protected:
		EventSubscriber() = default;
	};
}

