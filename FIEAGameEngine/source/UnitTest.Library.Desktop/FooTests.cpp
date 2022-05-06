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
	TEST_CLASS(FooTests)
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
			Foo foo1(1);
			Assert::AreEqual(foo1.Data(), 1);
		}

		TEST_METHOD(Destructor)
		{
			Foo* foo1 = new Foo(1);
			delete foo1;
		}

		TEST_METHOD(CopyConstructor)
		{
			Foo foo1(1);
			
			Foo foo2 = foo1;

			Assert::AreEqual(foo1.Data(), foo2.Data());
			Assert::AreNotSame(foo1, foo2);
		}

		TEST_METHOD(EqualOperator)
		{
			Foo foo1(1);
			Foo foo2(2);

			foo1 = foo2;

			Assert::AreEqual(foo1.Data(), foo2.Data());
			Assert::AreNotSame(foo1, foo2);
		}

		TEST_METHOD(Data)
		{
			Foo foo1(1);

			Assert::AreEqual(foo1.Data(), 1);

			std::int32_t& dataChanger = foo1.Data();
			dataChanger++;

			Assert::AreEqual(foo1.Data(), 2);
		}

		TEST_METHOD(DataConst)
		{
			const Foo foo1(1);

			Assert::AreEqual(foo1.Data(), 1);
		}

		TEST_METHOD(EqualTo)
		{
			Foo foo1(1);
			Foo foo2(1);

			bool isSame = foo1 == foo1;
			Assert::AreEqual(true, isSame);

			isSame = foo1 == foo2;
			Assert::AreEqual(true, isSame);
		}

		TEST_METHOD(NotEqualTo)
		{
			Foo foo1(1);
			Foo foo2(2);

			bool isSame = foo1 != foo1;
			Assert::AreEqual(false, isSame);

			isSame = foo1 != foo2;
			Assert::AreEqual(true, isSame);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FooTests::_startMemState;
}