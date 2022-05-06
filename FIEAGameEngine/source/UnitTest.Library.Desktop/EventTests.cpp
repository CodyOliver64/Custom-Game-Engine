#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"

#include "WorldManager.h"
#include "Event.h"
#include "FooSubscriber.h"
#include "EventFoo.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;
using namespace std::chrono_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
}

namespace FIEAGameEngine
{
	template<>
	class DefaultHash<Foo>
	{
	public:
		std::size_t operator()(const Foo& key) const
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(&key.Data()), sizeof(std::int32_t));
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTests)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined (_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined (_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}
		
		struct EventQueueSubscriber final : EventSubscriber
		{
			std::size_t _count = 0;
			void Notify(const EventPublisher& publisher)override
			{
				
				++_count;
				const Event<EventFoo>* event1 = publisher.As<Event<EventFoo>>();
				if (event1 != nullptr)
				{
					EventFoo message = event1->GetMessage();
					EventQueue& queue = message._gameState->_eventQueue;
					auto newEvent = std::make_shared<Event<EventFoo>>(message);
					GameTime gameTime;
					queue.Enqueue(newEvent, gameTime);
				}
				
			}
		};

		struct EventClearSubscriber final : EventSubscriber
		{
			std::size_t _count = 0;
			void Notify(const EventPublisher& publisher)override
			{

				++_count;
				const Event<EventFoo>* event1 = publisher.As<Event<EventFoo>>();
				if (event1 != nullptr)
				{
					EventFoo message = event1->GetMessage();
					EventQueue& queue = message._gameState->_eventQueue;
				
					queue.Clear();
				}

			}
		};
		
		TEST_METHOD(RTTITest)
		{
			Foo foo{ 1 };
			Event<Foo> event(foo);

			RTTI* rtti = &event;
			Assert::IsTrue(rtti->Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(rtti->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));

			Event<Foo>* e = rtti->As<Event<Foo>>();
			Assert::IsNotNull(e);
			Assert::AreEqual(&event, e);

			EventPublisher* ep = rtti->As<EventPublisher>();
			Assert::IsNotNull(ep);
			Assert::AreEqual(&event, static_cast<Event<Foo>*>(ep));
			Assert::IsTrue(rtti->Is(ep->TypeIdInstance()));

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(PublisherRTTITest)
		{
			Foo foo{ 1 };
			Event<Foo> event(foo);
			EventPublisher* eventPublisher = static_cast<EventPublisher*>(&event);

			RTTI* rtti = eventPublisher;
			Assert::IsFalse(rtti->Is("Foo"));
			Assert::IsTrue(rtti->Is("EventPublisher"));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(eventPublisher->TypeIdInstance(), rtti->TypeIdInstance());

			Foo* foo2 = rtti->As<Foo>();
			Assert::IsNull(foo2);

			EventPublisher* publisher2 = rtti->As<EventPublisher>();
			Assert::IsNotNull(publisher2);
			Assert::AreEqual(eventPublisher, publisher2);
			RTTI* rtti2 = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(rtti2);
			rtti2 = rtti->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsNotNull(rtti2);

			rtti2 = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(rtti2);

			rtti2 = rtti->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsNotNull(rtti2);
		}
		
		TEST_METHOD(EventClearEvent)
		{
			GameTime gameTime;
			WorldManager gameState;

			Foo f;
			EventClearSubscriber subsrciber;
			EventFoo eventFoo(&gameState);
			FooSubscriber fooSubscriber;

			std::shared_ptr<Event<EventFoo>> eef = std::make_shared<Event<EventFoo>>(eventFoo);
			std::shared_ptr<Event<Foo>> e = std::make_shared<Event<Foo>>(f);

			gameState._eventQueue.Enqueue(eef, gameTime);
			gameState._eventQueue.Enqueue(e, gameTime);

			Event<EventFoo>::Subscribe(subsrciber);
			Event<Foo>::Subscribe(fooSubscriber);

			gameState._eventQueue.Update(gameTime);

			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			Assert::AreEqual((std::size_t)0, subsrciber._count);
			Assert::AreEqual(gameState._eventQueue.Size(), (std::size_t)2);

			gameState._eventQueue.Update(gameTime);
			Assert::AreEqual((std::size_t)1, subsrciber._count);
			Assert::AreEqual(gameState._eventQueue.Size(), (std::size_t)0);
			gameState._eventQueue.Update(gameTime);
			Assert::AreEqual(gameState._eventQueue.Size(), (std::size_t)0);

			Event<EventFoo>::UnsubscribeAll();
			Event<Foo>::UnsubscribeAll();
		}
		
		TEST_METHOD(EventAddEvent)
		{
			GameTime gameTime;
			WorldManager gameState;

			EventQueueSubscriber subscriber;
			EventFoo eventFoo(&gameState);
			std::shared_ptr<Event<EventFoo>> event1 = std::make_shared<Event<EventFoo>>(eventFoo);

			gameState._eventQueue.Enqueue(event1, gameTime);
			Event<EventFoo>::Subscribe(subscriber);
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			Assert::AreEqual(std::size_t(0), subscriber._count);
			gameState._eventQueue.Update(gameTime);
			Assert::AreEqual(gameState._eventQueue.Size(), std::size_t(1));
			gameState._eventQueue.Update(gameTime);
			Assert::AreEqual(std::size_t(1), subscriber._count);
			Assert::AreEqual(gameState._eventQueue.Size(), std::size_t(1));
			gameState._eventQueue.Update(gameTime);
			Assert::AreEqual(std::size_t(2), subscriber._count);
			Assert::AreEqual(gameState._eventQueue.Size(), std::size_t(1));
			Event<EventFoo>::UnsubscribeAll();
		}

		TEST_METHOD(WorldManagerTest)
		{
			Foo testFoo;
			GameObject testObj;
			WorldManager testState(&testObj);
			FooSubscriber fooSubscriber;

			std::shared_ptr<Event<Foo>> testEvent = std::make_shared<Event<Foo>>(testFoo);

			Assert::AreEqual(size_t(0), testState._eventQueue.PendingSize());
			testState._eventQueue.Enqueue(testEvent, testState._time);
			Assert::AreEqual(size_t(1), testState._eventQueue.PendingSize());

			testState._gameClockEnabled = true;
			testState.Update();
			Assert::AreEqual(size_t(1), testState._eventQueue.Size());
			Assert::AreEqual(size_t(0), testState._eventQueue.PendingSize());

			Event<Foo>::Subscribe(fooSubscriber);
			testState._time.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			Assert::IsFalse(fooSubscriber._isNotified);
			testState.Update();
			Assert::IsTrue(fooSubscriber._isNotified);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(UnsubscribeTest)
		{
			GameTime time;
			EventQueue queue;
			FooSubscriber fooSubscriber1;
			FooSubscriber fooSubscriber2;
			FooSubscriber fooSubscriber3;

			Event<Foo>::Subscribe(fooSubscriber1);
			Event<Foo>::Subscribe(fooSubscriber2);
			Event<Foo>::Subscribe(fooSubscriber3);

			Assert::AreEqual(size_t(3), Event<Foo>::SubscriberListSize());

			Event<Foo>::Unsubscribe(fooSubscriber1);

			Assert::AreEqual(size_t(2), Event<Foo>::SubscriberListSize());

			Event<Foo>::UnsubscribeAll();

			Assert::AreEqual(size_t(0), Event<Foo>::SubscriberListSize());

		}
		
		TEST_METHOD(ClearQueue)
		{
			GameTime time;
			EventQueue queue;
			Assert::AreEqual(size_t(0), queue.Size());

			for (int i = 0; i < 4; ++i)
			{
				queue.Enqueue(std::make_shared<Event<Foo>>(Foo{}), time);
			}
			Assert::AreEqual(size_t(0), queue.Size());
			queue.Update(time);
			Assert::AreEqual(size_t(4), queue.Size());

			queue.Clear();
			Assert::AreEqual(size_t(4), queue.Size());
			queue.Update(time);
			Assert::AreEqual(size_t(0), queue.Size());
			Assert::IsTrue(queue.IsEmpty());

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(CopyTests)
		{
			{
				Event<Foo> event1(Foo{});
				Event<Foo> event2(event1);
				Assert::AreNotSame(event1.GetMessage(), event2.GetMessage());
				Assert::AreEqual(event1.GetMessage(), event2.GetMessage());
			}

			{
				Event<Foo> event1(Foo{});
				Event<Foo> event2(Foo{});
				event2 = event1;
				Assert::AreNotSame(event1.GetMessage(), event2.GetMessage());
				Assert::AreEqual(event1.GetMessage(), event2.GetMessage());
			}
		}

		TEST_METHOD(MoveTests)
		{
			{
				Event<Foo> event1(Foo{});
				Event<Foo> event2(std::move(event1));
				event2 = std::move(event1);
				Assert::IsFalse(&event1.GetMessage() == &event2.GetMessage());
			}

			{
				Event<Foo> event1(Foo{});
				Event<Foo> event2(Foo{});
				Assert::IsFalse(&event1.GetMessage() == &event2.GetMessage());
			}
		}

		TEST_METHOD(EnqueueDelay)
		{
			GameTime time;
			Foo testFoo;
			FooSubscriber fooSubscriber;
			EventQueue queue;

			std::shared_ptr<Event<Foo>> testEvent = std::make_shared<Event<Foo>>(testFoo);

			Assert::AreEqual(testEvent->GetMessage(), testFoo);

			Assert::AreEqual(size_t(0), queue.PendingSize());
			queue.Enqueue(testEvent, time);
			Assert::AreEqual(size_t(1), queue.PendingSize());

			queue.Update(time);
			Assert::AreEqual(size_t(1), queue.Size());
			Assert::AreEqual(size_t(0), queue.PendingSize());

			Event<Foo>::Subscribe(fooSubscriber);
			time.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			Assert::IsFalse(fooSubscriber._isNotified);
			queue.Update(time);
			Assert::IsTrue(fooSubscriber._isNotified);

			fooSubscriber._isNotified = false;
			queue.Update(time);
			Assert::IsFalse(fooSubscriber._isNotified);
			Assert::AreEqual(size_t(0), queue.Size());

			time.SetCurrentTime(std::chrono::high_resolution_clock::time_point());
			queue.Enqueue(testEvent, time, 1s);
			queue.Update(time);
			Assert::AreEqual(size_t(1), queue.Size());

			time.SetCurrentTime(std::chrono::high_resolution_clock::time_point(500ms));
			queue.Update(time);
			Assert::IsFalse(fooSubscriber._isNotified);

			time.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			queue.Update(time);
			Assert::IsFalse(fooSubscriber._isNotified);

			time.SetCurrentTime(std::chrono::high_resolution_clock::time_point(2s));
			queue.Update(time);
			Assert::IsTrue(fooSubscriber._isNotified);

			Event<Foo>::UnsubscribeAll();
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState EventTests::_startMemState;
}
