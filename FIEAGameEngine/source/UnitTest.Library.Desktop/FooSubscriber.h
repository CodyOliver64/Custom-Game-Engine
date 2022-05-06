#pragma once

#include <cstdint>
#include "RTTI.h"
#include "Event.h"
#include "Foo.h"

namespace FIEAGameEngine
{
	class FooSubscriber final : public EventSubscriber
	{
	public:
		void Notify(const EventPublisher& event) override;

		bool _isNotified{ false };
		int _data{ 0 };
	};
}


