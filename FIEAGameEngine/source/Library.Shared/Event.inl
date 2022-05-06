#include "Event.h"

namespace FIEAGameEngine
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>)
	
	template <typename T>
	Event<T>::Event(const T& eventInstance) :
		EventPublisher(_subscribers), _eventInstance(eventInstance)
	{
		
	}

	template <typename T>
	Event<T>::Event(T&& eventInstance) :
		EventPublisher(_subscribers)
	{
		_eventInstance = std::move(eventInstance);
	}

	template <typename T>
	void Event<T>::Subscribe(EventSubscriber& newSubscriber)
	{
		if (_subscribers.Find(&newSubscriber) == _subscribers.end())
		{
			_subscribers.PushBack(&newSubscriber);
		}
	}

	template <typename T>
	void Event<T>::Unsubscribe(EventSubscriber& newSubscriber)
	{
		_subscribers.Remove(&newSubscriber);
	}

	template <typename T>
	void Event<T>::UnsubscribeAll()
	{
		_subscribers.Clear();
	}

	template <typename T>
	const T& Event<T>::GetMessage() const
	{
		return _eventInstance;
	}

	template <typename T>
	size_t Event<T>::SubscriberListSize()
	{
		return _subscribers.Size();
	}
}