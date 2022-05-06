#pragma once
#include "EventPublisher.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Event is a template class that allows for the grouping of families of events.
	/// This class will serve as the interface for managing subscribers to the family of events, where every instance of an event will share a static list of subscribers 
	/// Each event will store a T message to be delivered to each subscriber
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event<T>, EventPublisher)
	public:
		Event(const T& eventInstance);
		Event(T&& eventInstance);


		/// <summary>
		/// Add a new subscriber to the list of subscribers for this event type
		/// </summary>
		/// <param name="newSubscriber">subscriber to be added </param>
		static void Subscribe(EventSubscriber& newSubscriber);

		/// <summary>
		/// Removes a subscriber from the list of subscribers for this event type
		/// </summary>
		/// <param name="newSubscriber">subscriber to be added </param>
		static void Unsubscribe(EventSubscriber& newSubscriber);
		
		/// <summary>
		/// Clears the list of subscribers for this event type
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Returns the current size of the list of subscribers to this event type
		/// </summary>
		static size_t SubscriberListSize();

		/// <summary>
		/// Returns the stored T (message) of the event as a constant reference
		/// </summary>
		const T& GetMessage() const;
		
	private:
		inline static Vector <EventSubscriber*> _subscribers;
		T _eventInstance;

	};
}

#include "Event.inl"
