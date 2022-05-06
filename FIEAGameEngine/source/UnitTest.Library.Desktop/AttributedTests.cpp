#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "AttributedThing.h"
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
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(&key.Data()), sizeof(std::int32_t));
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTests)
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


		TEST_METHOD(IsAttribute)
		{
			AttributedFoo aFoo1;
			
			Assert::IsTrue(aFoo1.IsAttribute("this"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalInteger"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalFloat"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalString"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalVector"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(aFoo1.IsAttribute("ExternalStringArray"));

			Assert::IsTrue(aFoo1.IsPrescribedAttribute("this"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(aFoo1.IsPrescribedAttribute("ExternalStringArray"));

			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(aFoo1.IsAuxiliaryAttribute("ExternalStringArray"));

			auto expression = []() { AttributedThing thing; };
			Assert::ExpectException<std::runtime_error>(expression);
		}

		TEST_METHOD(AuxillaryAppend)
		{
			AttributedFoo aFoo1;

			aFoo1.AppendAuxiliaryAttribute("AuxInteger");

			Assert::IsTrue(aFoo1.IsAttribute("AuxInteger"));
			Assert::IsFalse(aFoo1.IsPrescribedAttribute("AuxInteger"));
			Assert::IsTrue(aFoo1.IsAuxiliaryAttribute("AuxInteger"));

			auto expression = [&aFoo1]() { aFoo1.AppendAuxiliaryAttribute("ExternalInteger"); };
			Assert::ExpectException<std::runtime_error>(expression);
		}

		TEST_METHOD(ExternalSetData)
		{
			AttributedFoo aFoo1;

			Datum* datum = aFoo1.Find("ExternalInteger");
			Assert::AreEqual(size_t(1), datum->Size());
			Assert::AreSame(datum->FrontInt(), aFoo1.ExternalInteger);

			int32_t newInt = 10;
			datum->Set(newInt);
			Assert::AreEqual(datum->FrontInt(), newInt);
			Assert::AreEqual(aFoo1.ExternalInteger, newInt);

			float newFloat = 11.0f;
			aFoo1.ExternalFloat = newFloat;
			Assert::AreEqual(aFoo1.ExternalFloat, newFloat);

			Datum* datum2 = aFoo1.Find("ExternalFloat");
			Assert::AreEqual(datum2->FrontFloat(), newFloat);
		}

		TEST_METHOD(CopyConstruction)
		{
			AttributedFoo aFoo1;

			aFoo1.AppendAuxiliaryAttribute("AuxInteger");

			int32_t newInt = 10;
			aFoo1.ExternalInteger = newInt;
			
			float newFloat = 11.0f;
			aFoo1.ExternalFloat = newFloat;

			AttributedFoo aFoo2 = aFoo1;

			Datum* datum = aFoo2.Find("ExternalInteger");
			Assert::AreEqual(aFoo2.ExternalInteger, newInt);
			Assert::AreEqual(datum->FrontInt(), newInt);

			Datum* datum2 = aFoo2.Find("ExternalFloat");
			Assert::AreEqual(datum2->FrontFloat(), newFloat);
			Assert::AreEqual(aFoo2.ExternalFloat, newFloat);

			Assert::IsTrue(aFoo2.IsAuxiliaryAttribute("AuxInteger"));

			Assert::IsTrue(aFoo2.IsPrescribedAttribute("this"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalStringArray"));

			int32_t newNewInt = 15;
			aFoo1.ExternalInteger = newNewInt;

			Assert::AreEqual(aFoo2.ExternalInteger, newInt);

			Assert::AreNotSame(aFoo2.ExternalInteger, aFoo1.ExternalInteger);
		}

		TEST_METHOD(CopyAssignment)
		{
			AttributedFoo aFoo1;

			aFoo1.AppendAuxiliaryAttribute("AuxInteger");

			int32_t newInt = 10;
			aFoo1.ExternalInteger = newInt;

			float newFloat = 11.0f;
			aFoo1.ExternalFloat = newFloat;

			AttributedFoo aFoo2;

			aFoo2.AppendAuxiliaryAttribute("OldAttribute");

			int32_t oldInt = 20;
			aFoo2.ExternalInteger = oldInt;

			float oldFloat = 22.0f;
			aFoo2.ExternalFloat = oldFloat;
			
			aFoo2 = aFoo1;
			
			
			Assert::IsFalse(aFoo2.IsAuxiliaryAttribute("OldAttribute"));
			
			Datum* datum = aFoo2.Find("ExternalInteger");
			Assert::AreEqual(aFoo2.ExternalInteger, newInt);
			Assert::AreEqual(datum->FrontInt(), newInt);

			Datum* datum2 = aFoo2.Find("ExternalFloat");
			Assert::AreEqual(datum2->FrontFloat(), newFloat);
			Assert::AreEqual(aFoo2.ExternalFloat, newFloat);

			Assert::IsTrue(aFoo2.IsAuxiliaryAttribute("AuxInteger"));

			Assert::IsTrue(aFoo2.IsPrescribedAttribute("this"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalStringArray"));

			int32_t newNewInt = 15;
			aFoo1.ExternalInteger = newNewInt;

			Assert::AreEqual(aFoo2.ExternalInteger, newInt);

			Assert::AreNotSame(aFoo2.ExternalInteger, aFoo1.ExternalInteger);
		}

		TEST_METHOD(MoveConstruction)
		{
			AttributedFoo aFoo1;

			aFoo1.AppendAuxiliaryAttribute("AuxInteger");

			int32_t newInt = 10;
			aFoo1.ExternalInteger = newInt;

			float newFloat = 11.0f;
			aFoo1.ExternalFloat = newFloat;

			AttributedFoo aFoo2 = std::move(aFoo1);

			Datum* datum = aFoo2.Find("ExternalInteger");
			Assert::AreEqual(aFoo2.ExternalInteger, newInt);
			Assert::AreEqual(datum->FrontInt(), newInt);

			Datum* datum2 = aFoo2.Find("ExternalFloat");
			Assert::AreEqual(datum2->FrontFloat(), newFloat);
			Assert::AreEqual(aFoo2.ExternalFloat, newFloat);

			Assert::IsTrue(aFoo2.IsAuxiliaryAttribute("AuxInteger"));

			Assert::IsTrue(aFoo2.IsPrescribedAttribute("this"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalStringArray"));
		}

		TEST_METHOD(MoveAssignment)
		{
			AttributedFoo aFoo1;

			aFoo1.AppendAuxiliaryAttribute("AuxInteger");

			int32_t newInt = 10;
			aFoo1.ExternalInteger = newInt;

			float newFloat = 11.0f;
			aFoo1.ExternalFloat = newFloat;

			AttributedFoo aFoo2;

			aFoo2.AppendAuxiliaryAttribute("OldAttribute");

			int32_t oldInt = 20;
			aFoo2.ExternalInteger = oldInt;

			float oldFloat = 22.0f;
			aFoo2.ExternalFloat = oldFloat;


			aFoo2 = std::move(aFoo1);

			Datum* datum = aFoo2.Find("ExternalInteger");
			Assert::AreEqual(aFoo2.ExternalInteger, newInt);
			Assert::AreEqual(datum->FrontInt(), newInt);

			Datum* datum2 = aFoo2.Find("ExternalFloat");
			Assert::AreEqual(datum2->FrontFloat(), newFloat);
			Assert::AreEqual(aFoo2.ExternalFloat, newFloat);

			Assert::IsTrue(aFoo2.IsAuxiliaryAttribute("AuxInteger"));

			Assert::IsTrue(aFoo2.IsPrescribedAttribute("this"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(aFoo2.IsPrescribedAttribute("ExternalStringArray"));
		}

		TEST_METHOD(InheritanceSignatures)
		{
			AttributedBar aBar1;

			Assert::IsTrue(aBar1.IsPrescribedAttribute("this"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(aBar1.IsPrescribedAttribute("ExternalStringArray"));

			Assert::IsTrue(aBar1.IsPrescribedAttribute("SuperExternalInteger"));
		}

		TEST_METHOD(RTTITest)
		{
			AttributedFoo aFoo1;

			RTTI* rtti = &aFoo1;
			Assert::IsFalse(rtti->Is("Foo"));
			Assert::IsTrue(rtti->Is("AttributedFoo"));
			Assert::IsTrue(rtti->Is("Attributed"));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), rtti->TypeIdInstance());

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&aFoo1, f);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&aFoo1), fAsAttributed);

			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsNotNull(r);

			AttributedFoo otherFoo;
			Assert::IsTrue(rtti->Equals(&otherFoo));

			Foo anotherFoo(100);
			Assert::IsFalse(rtti->Equals(&anotherFoo));

			Assert::IsTrue("AttributedFoo" == r->ToString());
		}

		TEST_METHOD(StringTests)
		{
			AttributedFoo aFoo1;
			Assert::IsTrue(aFoo1.ToString() == "AttributedFoo");

			AttributedBar aBar1;
			Assert::IsTrue(aBar1.ToString() == "AttributedBar");
		}

		TEST_METHOD(EqualsTest)
		{
			AttributedFoo aFoo1;
			aFoo1.ExternalInteger = 20;
			aFoo1.AppendAuxiliaryAttribute("AuxAttribute");

			AttributedFoo aFoo2;
			aFoo2.ExternalInteger = 20;
			aFoo2.AppendAuxiliaryAttribute("AuxAttribute");

			Assert::IsTrue(aFoo1.Equals(&aFoo2));

			AttributedBar aBar1;
			aBar1.ExternalInteger = 20;
			aBar1.AppendAuxiliaryAttribute("AuxAttribute");

			AttributedBar aBar2;
			aBar2.ExternalInteger = 20;
			aBar2.AppendAuxiliaryAttribute("AuxAttribute");

			Assert::IsTrue(aBar1.Equals(&aBar2));
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState AttributedTests::_startMemState;
}