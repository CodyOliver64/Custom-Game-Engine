#include "pch.h"
#include "FooSubscriber.h"

using namespace std;

namespace FIEAGameEngine
{
	void FooSubscriber::Notify(const EventPublisher& event)
	{
		const Event<Foo>* fooEvent = event.As<Event<Foo>>();

		if (fooEvent != nullptr)
		{
			const Foo& foo = fooEvent->GetMessage();
			_data = foo.Data();
		}

		_isNotified = true; 
	}

}
