#include "pch.h"
#include "Foo.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"

#include "Factory.h"


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
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(&key.Data()), sizeof(std::int32_t));
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTests)
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


		TEST_METHOD(Creation)
		{
			RTTI* test = Factory<RTTI>::Create("Foo");
			Assert::IsNull(test);

			{
				FooFactory factory;

				test = Factory<RTTI>::Create("Foo");
				Assert::IsNotNull(test);

				Foo* foo = test->As<Foo>();
				Assert::IsNotNull(foo);

				delete foo;
			}

			test = Factory<RTTI>::Create("Foo");
			Assert::IsNull(test);
		}

		TEST_METHOD(ProductCreationAndAdding)
		{
			RTTI* rtti = Factory<RTTI>::Create("Foo");
			Assert::IsNull(rtti);
			{
				FooFactory fooFctory;
				rtti = Factory<RTTI>::Create("Foo");
				Assert::IsNotNull(rtti);
				Foo* foo = rtti->As<Foo>();
				Assert::IsNotNull(foo);
				delete foo;
			}

			rtti = Factory<RTTI>::Create("Foo");
			Assert::IsNull(rtti);


		}

		TEST_METHOD(AddRemoveFactory)
		{
			const Factory<RTTI>* foundFactory = Factory<RTTI>::Find("Foo");
			Assert::IsNull(foundFactory);

			{
				const FooFactory fooFactory;
				foundFactory = Factory<RTTI>::Find("Foo");
				Assert::AreEqual(&fooFactory, static_cast<const FooFactory*>(foundFactory));
			}

			foundFactory = Factory<RTTI>::Find("Foo");
			Assert::IsNull(foundFactory);

			const FooFactory fooFactory;
			Assert::ExpectException<std::runtime_error>([] {FooFactory fooFactory; });
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FactoryTests::_startMemState;
}