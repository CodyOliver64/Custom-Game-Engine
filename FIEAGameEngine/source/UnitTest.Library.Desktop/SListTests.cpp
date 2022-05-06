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
	TEST_CLASS(SListTests)
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


		
		TEST_METHOD(Constructor)
		{
			SList<Foo> list;
			Assert::AreEqual((size_t)0, list.Size());
		}

		TEST_METHOD(Destructor)
		{
			SList<Foo>* list  = new SList<Foo>;
			Foo foo1(6);
			list->PushFront(foo1);

			delete list;
		}

		TEST_METHOD(Front)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;

			auto expression = [&list]() { list.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(foo1);
			Assert::AreEqual(foo1, list.Front());
			Assert::AreEqual(foo1, list.Back());

			list.PushFront(foo2);
			Assert::AreEqual(foo1, list.Back());
			Assert::AreEqual(foo2, list.Front());

			list.PushFront(foo3);
			Assert::AreEqual(foo1, list.Back());
			Assert::AreEqual(foo3, list.Front());
		}

		TEST_METHOD(FrontConst)
		{
			const SList<Foo> emptyList;
			auto expression = [&emptyList]() { emptyList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);
			
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;
			list.PushFront(foo3);
			list.PushFront(foo2);
			list.PushFront(foo1);

			const SList<Foo> list2 = list;

			Assert::AreEqual(foo1, list2.Front());
		}

		TEST_METHOD(Back)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;

			auto expression = [&list]() { list.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushBack(foo1);
			Assert::AreEqual(foo1, list.Front());
			Assert::AreEqual(foo1, list.Back());

			list.PushBack(foo2);
			Assert::AreEqual(foo1, list.Front());
			Assert::AreEqual(foo2, list.Back());

			list.PushBack(foo3);
			Assert::AreEqual(foo1, list.Front());
			Assert::AreEqual(foo3, list.Back());
		}

		TEST_METHOD(BackConst)
		{
			const SList<Foo> emptyList;
			auto expression = [&emptyList]() { emptyList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;
			list.PushFront(foo1);
			list.PushFront(foo2);
			list.PushFront(foo3);

			const SList<Foo> list2 = list;

			Assert::AreEqual(foo1, list2.Back());
		}

		TEST_METHOD(PushFront)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			list.PushFront(foo1);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(foo1, list.Front());
			Assert::AreEqual(foo1, list.Back());
			Assert::AreSame(list.Front(), list.Back());

			list.PushFront(foo2);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(foo2, list.Front());
			Assert::AreEqual(foo1, list.Back());

			list.PushFront(foo3);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(foo3, list.Front());
			Assert::AreEqual(foo1, list.Back());
		}

		TEST_METHOD(PushBack)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			list.PushBack(foo1);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(foo1, list.Front());
			Assert::AreEqual(foo1, list.Back());
			Assert::AreSame(list.Front(), list.Back());

			list.PushBack(foo2);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(foo2, list.Back());
			Assert::AreEqual(foo1, list.Front());

			list.PushBack(foo3);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(foo3, list.Back());
			Assert::AreEqual(foo1, list.Front());
		}

		TEST_METHOD(PopFront)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;

			auto expression = [&list]() { list.PopFront(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(foo3);
			list.PushFront(foo2);
			list.PushFront(foo1);

			Assert::AreEqual(foo1, list.Front());
			list.PopFront();
			Assert::AreEqual(foo2, list.Front());
			list.PopFront();
			Assert::AreEqual(foo3, list.Front());
			list.PopFront();

			Assert::IsTrue(list.IsEmpty());
		}
		
		TEST_METHOD(PopBack)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;

			auto expression = [&list]() { list.PopBack(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(foo3);
			list.PushFront(foo2);
			list.PushFront(foo1);

			Assert::AreEqual(foo3, list.Back());
			list.PopBack();
			Assert::AreEqual(foo2, list.Back());
			list.PopBack();
			Assert::AreEqual(foo1, list.Back());
			list.PopBack();

			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(IsEmpty)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;

			Assert::AreEqual(true, list.IsEmpty());

			list.PushFront(foo3);
			list.PushFront(foo2);
			list.PushFront(foo1);


			Assert::AreEqual(false, list.IsEmpty());

			list.Clear();

			Assert::AreEqual(true, list.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;

			Assert::AreEqual(size_t(0), list.Size());

			list.PushFront(foo3);
			list.PushFront(foo2);
			list.PushFront(foo1);


			Assert::AreEqual(size_t(3), list.Size());

			list.Clear();

			Assert::AreEqual(size_t(0), list.Size());
		}
		TEST_METHOD(Clear)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			SList<Foo> list;

			list.Clear();
			Assert::AreEqual(size_t(0), list.Size());


			list.PushFront(foo3);
			list.PushFront(foo2);
			list.PushFront(foo1);

			Assert::AreEqual(size_t(3), list.Size());

			list.Clear();

			Assert::AreEqual(size_t(0), list.Size());
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState SListTests::_startMemState;
}
