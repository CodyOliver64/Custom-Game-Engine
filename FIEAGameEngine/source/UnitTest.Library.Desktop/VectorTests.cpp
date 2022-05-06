#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "ToStringSpecializations.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTests)
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
			Vector<Foo> list;
			Assert::AreEqual((size_t)0, list.Size());
		}

		TEST_METHOD(Destructor)
		{
			Vector<Foo>* list = new Vector<Foo>;
			Foo foo1(6);
			list->PushBack(foo1);

			delete list;
		}

		TEST_METHOD(FrontBack)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;

			auto expression = [&list]() { list.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			auto expression2 = [&list]() { list.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);

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

		TEST_METHOD(FrontConst)
		{
			const Vector<Foo> emptyList;
			auto expression = [&emptyList]() { emptyList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;
			list.PushBack(foo3);
			list.PushBack(foo2);
			list.PushBack(foo1);

			const Vector<Foo> list2 = list;

			Assert::AreEqual(foo3, list2.Front());
		}

		TEST_METHOD(BackConst)
		{
			const Vector<Foo> emptyList;
			auto expression = [&emptyList]() { emptyList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;
			list.PushBack(foo3);
			list.PushBack(foo2);
			list.PushBack(foo1);

			const Vector<Foo> list2 = list;

			Assert::AreEqual(foo1, list2.Back());
		}

		TEST_METHOD(PushBack)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;
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

		TEST_METHOD(PopBack)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;

			auto expression = [&list]() { list.PopBack(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

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

			Vector<Foo> list;

			Assert::AreEqual(true, list.IsEmpty());

			list.PushBack(foo3);
			list.PushBack(foo2);
			list.PushBack(foo1);


			Assert::AreEqual(false, list.IsEmpty());

			list.Clear();

			Assert::AreEqual(true, list.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;

			Assert::AreEqual(size_t(0), list.Size());

			list.PushBack(foo3);
			list.PushBack(foo2);
			list.PushBack(foo1);


			Assert::AreEqual(size_t(3), list.Size());

			list.PopBack();

			Assert::AreEqual(size_t(2), list.Size());

			list.Clear();

			Assert::AreEqual(size_t(0), list.Size());
		}
		TEST_METHOD(Clear)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;

			list.Clear();
			Assert::AreEqual(size_t(0), list.Size());


			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			Assert::AreEqual(size_t(3), list.Size());

			list.Clear();

			Assert::AreEqual(size_t(0), list.Size());
		}

		TEST_METHOD(BeginEndEqual)
		{
			Vector<Foo> list;
			Assert::AreEqual(list.end(), list.begin());
			Assert::IsFalse(list.end() != list.begin());

			Foo foo1(10);
			list.PushBack(foo1);

			Vector<Foo>::Iterator it1 = list.begin();
			Vector<Foo>::Iterator it2 = list.end();

			Assert::AreNotEqual(list.end(), list.begin());
			Assert::IsTrue(list.end() != list.begin());
		}

		TEST_METHOD(ConstBeginEndEqual)
		{
			Vector<Foo> list;
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

			Vector<Foo>::Iterator nullIt;
			auto expression = [&nullIt]() { nullIt++; };
			auto expression2 = [&nullIt]() { ++nullIt; };
			Assert::ExpectException<std::runtime_error>(expression);
			Assert::ExpectException<std::runtime_error>(expression2);

			Vector<Foo>::ConstIterator constnullIt;
			auto expression3 = [&constnullIt]() { constnullIt++; };
			auto expression4 = [&constnullIt]() { ++constnullIt; };
			Assert::ExpectException<std::runtime_error>(expression3);
			Assert::ExpectException<std::runtime_error>(expression4);

			Vector<Foo> list;
			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);
			Vector<Foo>::Iterator it = list.begin();


			Assert::AreEqual(*it, foo1);

			++it;
			Assert::AreEqual(*it, foo2);

			Vector<Foo>::Iterator it2 = it++;
			Assert::AreNotEqual(*it, *it2);
			Assert::AreEqual(*it2, foo2);
			Assert::AreEqual(*it, foo3);
		}

		TEST_METHOD(ConstIncrement)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;
			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			const Vector<Foo> list2 = list;

			Vector<Foo>::ConstIterator it = list2.begin();


			Assert::AreEqual(*it, foo1);

			++it;
			Assert::AreEqual(*it, foo2);

			Vector<Foo>::ConstIterator it2 = it++;
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

			Vector<Foo> list;

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

			const Vector<Foo> constList;
			Vector<Foo> list;

			Assert::AreEqual(constList.end(), constList.Find(foo1));

			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			const Vector<Foo> constList2 = list;

			Assert::AreEqual(constList2.end(), constList2.Find(fooNotHere));

			Assert::AreEqual(foo1, *(constList2.Find(foo1)));
		}

		TEST_METHOD(Remove)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;


			list.Remove(foo1);

			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			list.Remove(foo1);
			Assert::AreEqual(*(list.begin()), foo2);

			//list.PushBack(foo1);

			list.Remove(foo2);
			Assert::AreEqual(*(list.begin()), foo3);

			//list.PushBack(foo3);

			list.Remove(foo3);
			Assert::AreEqual(list.Find(foo2), list.end());


			const Foo foo4(8);
			list.Remove(foo4);
		}

		TEST_METHOD(Remove2)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;

			Vector<Foo>::Iterator it = list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			Vector<Foo>::Iterator it2 = list.Find(foo2);

			list.Remove(it2);
		}

		TEST_METHOD(MoveConstructor)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;

			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);
			
			Vector<Foo> list2 = std::move(list);
		}

		TEST_METHOD(MoveOperator)
		{
			const Foo foo1(5);
			const Foo foo2(6);
			const Foo foo3(7);

			Vector<Foo> list;

			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			Vector<Foo> list2;

			list2 = std::move(list);
		}

		TEST_METHOD(Reserve)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);
			const Foo foo4(4);
			const Foo foo5(5);
			const Foo foo6(6);
			const Foo foo7(7);
			const Foo foo8(8);
			const Foo foo9(9);
			const Foo foo10(10);
			const Foo foo11(11);

			Vector<Foo> list;

			Assert::AreEqual(list.Size(), size_t(0));
			Assert::AreEqual(list.Capacity(), size_t(11));

			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);
			list.PushBack(foo4);
			list.PushBack(foo5);
			list.PushBack(foo6);
			list.PushBack(foo7);
			list.PushBack(foo8);
			list.PushBack(foo9);

			Assert::AreEqual(list.Size(), size_t(9));
			Assert::AreEqual(list.Capacity(), size_t(11));

			list.PushBack(foo10);

			Assert::AreEqual(list.Size(), size_t(10));
			Assert::AreEqual(list.Capacity(), size_t(11));

			list.PushBack(foo11);

			Assert::AreEqual(list.Size(), size_t(11));
			Assert::AreEqual(list.Capacity(), size_t(22));
		}

		TEST_METHOD(At_Brackets)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);

			Vector<Foo> list;

			auto expression = [&list]() { list.At(5); };
			auto expression2 = [&list]() { list[5]; };
			Assert::ExpectException<std::runtime_error>(expression);
			Assert::ExpectException<std::runtime_error>(expression2);


			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			Assert::ExpectException<std::runtime_error>(expression);
			Assert::ExpectException<std::runtime_error>(expression2);

			Assert::AreEqual(foo2, list.At(1));
			Assert::AreEqual(foo3, list[2]);

			const Vector<Foo> constList = list;

			auto expression3 = [&constList]() { constList.At(5); };
			auto expression4 = [&constList]() { constList[5]; };
			Assert::ExpectException<std::runtime_error>(expression3);
			Assert::ExpectException<std::runtime_error>(expression4);

			Assert::AreEqual(foo2, constList.At(1));
			Assert::AreEqual(foo3, constList[2]);

		}

		TEST_METHOD(Assignment)
		{
			const Foo foo1(1);
			const Foo foo2(2);
			const Foo foo3(3);
			const Foo foo4(4);
			const Foo foo5(5);
			const Foo foo6(6);
			const Foo foo7(7);
			const Foo foo8(8);
			const Foo foo9(9);
			const Foo foo10(10);
			const Foo foo11(11);

			Vector<Foo> list;
			Vector<Foo> list2;

			list.PushBack(foo1);
			list.PushBack(foo2);
			list.PushBack(foo3);

			list2 = list;

			Assert::AreEqual(list[1], list2[1]);

			
			list.PushBack(foo4);
			list.PushBack(foo5);
			list.PushBack(foo6);
			list.PushBack(foo7);
			list.PushBack(foo8);
			list.PushBack(foo9);
			list.PushBack(foo10);
			list.PushBack(foo11);

			list2 = list;

			Assert::AreEqual(list[10], list2[10]);

			Vector<Foo> list3;
			list = list3;

			Assert::AreEqual(list.IsEmpty(), true);
		}

		

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState VectorTests::_startMemState;
}