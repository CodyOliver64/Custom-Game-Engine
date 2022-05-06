#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"

#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "AttributedFoo.h"


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
	TEST_CLASS(TableParseHelperTests)
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

		TEST_METHOD(ExternalArrayUpdate)
		{
			const std::string filename = "../../../../../source/UnitTest.Library.Desktop/TestJson4.json";


			JsonTableParseHelper::SharedData sharedData;

			JsonTableParseHelper scopeParser;

			JsonParseCoordinator parseManager(sharedData);
			parseManager.AddHelper(scopeParser);

			AttributedFooFactory fooFactory;
			ScopeFactory scopeFactory;

			parseManager.ParseFromFile(filename);

			Datum* cWeapon = sharedData._rootScope->Find("TheFoo");
			Assert::IsNotNull(cWeapon);

			AttributedFoo* foo = cWeapon->GetScope()->As<AttributedFoo>();
			Assert::IsNotNull(foo);

			Assert::IsTrue(foo->IsPrescribedAttribute("this"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalStringArray"));

			Assert::IsTrue(foo->IsAuxiliaryAttribute("Ammo"));

			Datum* externalIntArrayDatum = foo->Find("ExternalIntegerArray");

			Assert::AreEqual(int32_t(11), externalIntArrayDatum->GetInt(0));
			Assert::AreEqual(int32_t(21), externalIntArrayDatum->GetInt(1));
			Assert::AreEqual(int32_t(31), externalIntArrayDatum->GetInt(2));
		}
		TEST_METHOD(AttributedFooTest)
		{
			const std::string filename = "../../../../../source/UnitTest.Library.Desktop/TestJson3.json";


			JsonTableParseHelper::SharedData sharedData;

			JsonTableParseHelper scopeParser;

			JsonParseCoordinator parseManager(sharedData);
			parseManager.AddHelper(scopeParser);

			AttributedFooFactory fooFactory;
			ScopeFactory scopeFactory;

			parseManager.ParseFromFile(filename);

			Datum* cWeapon = sharedData._rootScope->Find("TheFoo");
			Assert::IsNotNull(cWeapon);

			AttributedFoo* foo = cWeapon->GetScope()->As<AttributedFoo>();
			Assert::IsNotNull(foo);

			Assert::IsTrue(foo->IsPrescribedAttribute("this"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo->IsPrescribedAttribute("ExternalStringArray"));

			Assert::IsTrue(foo->IsAuxiliaryAttribute("Gun"));
			Assert::IsTrue(foo->IsAuxiliaryAttribute("Ammo"));

		}
		
		
		TEST_METHOD(ArrayOfScopes)
		{
			const std::string filename = "../../../../../source/UnitTest.Library.Desktop/TestJson2.json";

			
			JsonTableParseHelper::SharedData sharedData;

			JsonTableParseHelper scopeParser;

			JsonParseCoordinator parseManager(sharedData);
			parseManager.AddHelper(scopeParser);

			ScopeFactory scopeFactory;

			parseManager.ParseFromFile(filename);

			Assert::AreEqual(sharedData._rootScope->Find("Weapons")->GetScope(0)->Find("Sword")->GetInt(0), 1);
			Assert::AreEqual(sharedData._rootScope->Find("Weapons")->GetScope(1)->Find("Spear")->GetInt(0), 4);
		}
		
		
		TEST_METHOD(FileParse)
		{
			
			const std::string filename = "../../../../../source/UnitTest.Library.Desktop/TestJson.json";

			JsonTableParseHelper::SharedData sharedData;

			JsonTableParseHelper tableHelper;
			
			JsonParseCoordinator parser(sharedData);
			parser.AddHelper(tableHelper);

			ScopeFactory scopeFactory;

			parser.ParseFromFile(filename);

			Datum* Name = sharedData._rootScope->Find("Name");
			Assert::IsNotNull(Name);

			Datum* Health = sharedData._rootScope->Find("Health");
			Assert::IsNotNull(Health);

			Datum* Damage = sharedData._rootScope->Find("Damage");
			Assert::IsNotNull(Damage);

			Datum* Direction = sharedData._rootScope->Find("Direction");
			Assert::IsNotNull(Direction);

			Datum* Transform = sharedData._rootScope->Find("ZTransform");
			Assert::IsNotNull(Transform);

			Datum* Aliases = sharedData._rootScope->Find("Aliases");
			Assert::IsNotNull(Aliases);



			Datum* Address = sharedData._rootScope->Find("Address");
			Scope* AddressScope = Address->GetScope();
			Assert::IsNotNull(AddressScope);

			Datum* Street = AddressScope->Find("Street");
			Assert::IsNotNull(Street);

			Datum* City = AddressScope->Find("City");
			Assert::IsNotNull(City);



			Datum* PostalCode = AddressScope->Find("Postal Code");
			Scope* PostalCodeScope = PostalCode->GetScope();
			Assert::IsNotNull(PostalCodeScope);

			Datum* ZipCode = PostalCodeScope->Find("Zip Code");
			Assert::IsNotNull(ZipCode);

			Datum* ZipCode4 = PostalCodeScope->Find("Zip Code+4");
			Assert::IsNotNull(ZipCode4);

			
			Assert::AreEqual(Name->FrontString(), std::string("Test"));
			Assert::AreEqual(Health->FrontInt(), 100);
			Assert::AreEqual(Damage->FrontFloat(), 100.0f);
			Assert::AreEqual(Direction->FrontVec(), glm::vec4(1,0,0,1));
			Assert::AreEqual(Aliases->GetString(0), std::string("Bob"));
			Assert::AreEqual(Aliases->GetString(1), std::string("Jim"));
			Assert::AreEqual(Aliases->GetString(2), std::string("Fred"));

			Assert::AreEqual(Street->FrontString(), std::string("123 Anystreet St."));
			Assert::AreEqual(City->FrontString(), std::string("Orlando"));

			Assert::AreEqual(ZipCode->FrontInt(), 12345);
			Assert::AreEqual(ZipCode4->FrontInt(), 6789);
			
		}

		TEST_METHOD(StringParse)
		{
			std::string input = R"({"Address":{"type":"table","value": {"class": "Scope", "Street":{"type":"integer","value":"1"}}}})";

			JsonTableParseHelper::SharedData sharedData;

			JsonTableParseHelper tableParser;

			JsonParseCoordinator parseCoordinator(sharedData);
			parseCoordinator.AddHelper(tableParser);

			ScopeFactory scopeFactory;

			parseCoordinator.ParseFromString(input);
			Assert::AreEqual(1, sharedData._rootScope->Find("Address")->GetScope()->Find("Street")->GetInt(0));
		}

		TEST_METHOD(Clone)
		{
			JsonTableParseHelper::SharedData sharedData;
			JsonParseCoordinator parser(sharedData);

			Assert::IsFalse(parser.IsClone());
			JsonTableParseHelper intHelper;
			parser.AddHelper(intHelper);

			auto clone = parser.Clone();
			Assert::AreNotEqual(clone, &parser);
			Assert::IsTrue(clone->IsClone());

			auto helperClone = clone->GetHelpers()[0]->As<JsonTableParseHelper>();

			Assert::IsNotNull(helperClone);
			Assert::AreNotEqual(helperClone, parser.GetHelpers()[0]->As<JsonTableParseHelper>());
			Assert::AreNotSame(clone->GetSharedData(), parser.GetSharedData());

			JsonTableParseHelper badHelper;
			clone->AddHelper(badHelper);
			Assert::AreEqual(size_t(1), clone->GetHelpers().Size());
			clone->RemoveHelper(*helperClone);
			Assert::AreEqual(size_t(1), clone->GetHelpers().Size());

			JsonTableParseHelper::SharedData badSharedData;
			Assert::IsFalse(clone->SetSharedData(&badSharedData));

			delete clone;
		}

		TEST_METHOD(BadSharedData)
		{
			
			std::string input = R"({"Address":{"type":"table","value": {"Street":{"type":"integer","value":"1"}}}})";

			JsonIntegerParseHelper::SharedData sharedData;
			JsonParseCoordinator parser(sharedData);

			Assert::IsFalse(parser.IsClone());
			JsonTableParseHelper intHelper;
			parser.AddHelper(intHelper);

			parser.ParseFromString(input);

			Assert::IsTrue(sharedData._data.IsEmpty());

			Assert::IsFalse(intHelper.EndHandler(sharedData, "", ""));
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState TableParseHelperTests::_startMemState;
}