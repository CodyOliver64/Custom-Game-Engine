#include "pch.h"
#include "SList.h"
#include "Foo.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

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
			return key.Data();
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DefaultHashTests)
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


		TEST_METHOD(IntHashTest)
		{
			int a = 1;
			int b = 2;
			int c = 1;

			DefaultHash<int> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(c), hashFunc(b));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}
		
		TEST_METHOD(StringHashTest)
		{
			std::string a = "YoYo";
			std::string b = "SupSup";
			std::string c(a);

			DefaultHash<std::string> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(c), hashFunc(b));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(FooHashTest)
		{
			Foo a(10);
			Foo b(11);
			Foo c(10);
			DefaultHash<Foo> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(c), hashFunc(b));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}
	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState DefaultHashTests::_startMemState;
}