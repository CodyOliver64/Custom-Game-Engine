#pragma once
#include <memory.h>
#include "EventPublisher.h"
#include "GameTime.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Class that holds a queue of events to be delivered during the root objects main update
	/// The class has its own update loop that will call Deliver on each Action in the queue, then update the queue
	/// </summary>
	class EventQueue final
	{

	public:
		
		/// <summary>
		/// Adds a new event to the pending event queue that is updated after the main Update loop finishes
		/// Will store a pointer to the event and time to delay the event past when it was added
		/// </summary>
		/// <param name="newEvent">Event to be stored in the queue</param>
		/// <param name="currentTime">current game time</param>
		/// <param name="delay">amount of time to delay the even delivery</param>
		void Enqueue(std::shared_ptr<EventPublisher> newEvent, const GameTime& currentTime, const std::chrono::milliseconds delay = std::chrono::milliseconds::zero());
		
		/// <summary>
		/// WIll deliver the events stored in the queue to all subscribers, removing the event after delivery
		/// After this the function will update the queue with any pending events or calls to Clear
		/// </summary>
		/// <param name="currentTime"></param>
		void Update(const GameTime& currentTime);
		
		/// <summary>
		/// Sets a flag that allows Update to clear the event queue after finishing an update loop
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns true of the queue is empty, false if it is not
		/// </summary>
		/// <returns>true or false</returns>
		bool IsEmpty() const;
		
		/// <summary>
		/// Returns the current size of the even queue
		/// </summary>
		/// <returns>size_t that is the size of the queue</returns>
		size_t Size() const;
		
		/// <summary>
		/// Returns the current size of the pending event queue
		/// </summary>
		/// <returns></returns>
		size_t PendingSize() const;

	private:
		struct QueueEntry final
		{
			std::shared_ptr<EventPublisher> Event;
			std::chrono::high_resolution_clock::time_point startTime;

			bool operator==(const QueueEntry& other) const
			{
				return (Event == other.Event && startTime == other.startTime);
			}
		};

		Vector<QueueEntry> _queue;
		Vector<QueueEntry> _deferredAddEventQueue;
		bool _clearRequired{ false };

	};
}

