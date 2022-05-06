#include "pch.h"
#include "SList.h"
#include "Foo.h"
#include "ToStringSpecializations.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(IteratorTests)
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



		TEST_METHOD(BeginEndEqual)
		{
			SList<Foo> list;
			Assert::AreEqual(list.end(), list.begin());
			Assert::IsFalse(list.end() != list.begin());

			Foo foo1(10);
			list.PushBack(foo1);

			Assert::AreNotEqual(list.end(), list.begin());
			Assert::IsTrue(list.end() != list.begin());
		}

		TEST_METHOD(ConstBeginEndEqual)
		{
			SList<Foo> list;
			Assert::AreEqual(list.cend(), list.cbegin());
			Assert::IsFalse(list.cend() != list.cbegin());

			Foo foo1(10);
			list.PushBack(foo1);

			Assert::AreNotEqual(list.cend(), list.cbegin());
			Assert::IsTrue(list.cend() != list.cbegin());
		}

		TEST_METHOD(Increment)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;
			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);
			SList<Foo>::Iterator it = list.begin();

			
			Assert::AreEqual(*it, foo1);

			++it;
			Assert::AreEqual(*it, foo2);

			SList<Foo>::Iterator it2 = it++;
			Assert::AreNotEqual(*it, *it2);
			Assert::AreEqual(*it2, foo2);
			Assert::AreEqual(*it, foo3);
		}

		TEST_METHOD(ConstIncrement)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;
			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			const SList<Foo> list2 = list;

			SList<Foo>::ConstIterator it = list2.begin();


			Assert::AreEqual(*it, foo1);

			++it;
			Assert::AreEqual(*it, foo2);

			SList<Foo>::ConstIterator it2 = it++;
			Assert::AreNotEqual(*it, *it2);
			Assert::AreEqual(*it2, foo2);
			Assert::AreEqual(*it, foo3);
		}

		TEST_METHOD(Find)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			const Foo fooNotHere(10);

			SList<Foo> list;

			Assert::AreEqual(list.end(), list.Find(foo1));

			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			Assert::AreEqual(list.end(), list.Find(fooNotHere));

			Assert::AreEqual(foo1, *(list.Find(foo1)));
		}

		TEST_METHOD(ConstFind)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			const Foo fooNotHere(10);

			const SList<Foo> constList;
			SList<Foo> list;

			Assert::AreEqual(constList.end(), constList.Find(foo1));

			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			const SList<Foo> constList2 = list;

			Assert::AreEqual(constList2.end(), constList2.Find(fooNotHere));

			Assert::AreEqual(foo1, *(constList2.Find(foo1)));
		}

		TEST_METHOD(InsertAfter)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);
			const Foo foo4(8);
			const Foo foo5(8);

			SList<Foo> list;
			SList<Foo> list2;


			auto expression = [&list, &list2, &foo1]() { list.InsertAfter(foo1, list2.begin()); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(foo3);
			list.PushFront(foo2);
			list.PushFront(foo1);

			SList<Foo>::Iterator it = list.begin();
			++it;
			++it;

			list.InsertAfter(foo4, it);
			Assert::AreEqual(foo4, *(++it));

			SList<Foo>::Iterator it2 = list.begin();
			++it2;

			list.InsertAfter(foo5, it2);
			Assert::AreEqual(foo5, *(++it2));
		}

		TEST_METHOD(Remove)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;

			
			auto expression = [&list, &foo1]() { list.Remove(foo1); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(foo3);
			list.PushFront(foo2);
			list.PushFront(foo1);

			list.Remove(foo1);
			Assert::AreEqual(*(list.begin()), foo2);

			list.PushFront(foo1);

			list.Remove(foo3);
			Assert::AreEqual(*(++list.begin()), foo2);

			list.PushBack(foo3);

			list.Remove(foo2);
			Assert::AreEqual(list.Find(foo2), list.end());

			
			const Foo foo4(8);
			list.Remove(foo4);
		}
		

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState IteratorTests::_startMemState;
}
