#include "pch.h"
#include "EventQueue.h"
#include <algorithm>
#include <assert.h>


namespace FIEAGameEngine
{

	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> newEvent, const GameTime& currentTime, const std::chrono::milliseconds delay)
	{
		_deferredAddEventQueue.PushBack(QueueEntry{ newEvent, currentTime.CurrentTime() + delay }); //Push/Pop back is LIFO, need to update to be a queue
	}

	void EventQueue::Update(const GameTime& currentTime)
	{
		auto it = std::partition(_queue.begin(), _queue.end(), [currentTime](QueueEntry& currentEvent) {return  currentTime.CurrentTime() <= currentEvent.startTime; });

		for (Vector<QueueEntry>::Iterator it1 = it; it1 != _queue.end(); ++it1)
		{
			(*it1).Event->Deliver();
		}
		_queue.Remove(it, _queue.end());

		//Update event queue
		if (_clearRequired)
		{
			_queue.Clear();
			_clearRequired = false;
		}

		for (QueueEntry& entry : _deferredAddEventQueue)
		{
			_queue.PushBack(QueueEntry{ entry.Event, entry.startTime });
		}
		_deferredAddEventQueue.Clear();
	}

	void EventQueue::Clear()
	{
		_clearRequired = true;
	}

	bool EventQueue::IsEmpty() const
	{
		return _queue.IsEmpty();
	}

	size_t EventQueue::Size() const
	{
		return _queue.Size();
	}

	size_t EventQueue::PendingSize() const
	{
		return _deferredAddEventQueue.Size();
	}

}
