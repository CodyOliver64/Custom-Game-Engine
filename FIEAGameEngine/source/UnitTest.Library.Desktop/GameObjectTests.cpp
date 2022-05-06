#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"

#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "GameObject.h"
#include "Monster.h"


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
	TEST_CLASS(GameObjectTests)
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

		TEST_METHOD(Level1)
		{
			const std::string filename = "../../../../../source/UnitTest.Library.Desktop/GameObjectJson.json";
			//GameObject rootObj;

			JsonTableParseHelper::SharedData sharedData(new GameObject);

			JsonTableParseHelper scopeParser;

			JsonParseCoordinator parseManager(sharedData);
			parseManager.AddHelper(scopeParser);

			AttributedFooFactory fooFactory;
			ScopeFactory scopeFactory;
			GameObjectFactory gameObjectFactory;
			MonsterFactory monsterFactory;

			parseManager.ParseFromFile(filename);

			Datum* name = sharedData._rootScope->Find("Name");
			Assert::IsNotNull(name);
			Assert::AreEqual(name->FrontString(), std::string("Level1"));

			
			Datum* transformDatum = sharedData._rootScope->Find("Transform");
			Assert::IsNotNull(transformDatum);
			Transform* transform = transformDatum->GetScope()->As<Transform>();
			Assert::IsNotNull(transform);

			Datum* position = transform->Find("Position");
			Datum* rotation = transform->Find("Rotation");
			Datum* scale = transform->Find("Scale");
			Assert::IsNotNull(position);
			Assert::IsNotNull(rotation);
			Assert::IsNotNull(scale);

			Assert::AreEqual(position->FrontVec(), glm::vec4(10.0, 20.0, 30.0, 40.0));
			Assert::AreEqual(rotation->FrontVec(), glm::vec4(40.0, 30.0, 20.0, 10.0));
			Assert::AreEqual(scale->FrontVec(), glm::vec4(60.0, 70.0, 80.0, 90.0));


			
			Datum* children = sharedData._rootScope->Find("Children");
			Assert::IsNotNull(children);
			Monster* monster = children->GetScope()->As<Monster>();
			Assert::IsNotNull(monster);


			Datum* health = monster->Find("Health");
			Assert::IsNotNull(health);
			Assert::AreEqual(health->FrontInt(), 75);

			GameObject* rootObj = sharedData._rootScope->As<GameObject>();
			Assert::IsNotNull(rootObj);

			WorldManager testState;
			rootObj->Update(testState);
			Assert::AreEqual(health->FrontInt(), 74);
		}

		TEST_METHOD(TestRTTI)
		{
			GameObject gameObject;

			RTTI* rtti = &gameObject;
			Assert::IsFalse(rtti->Is("Foo"));
			Assert::IsTrue(rtti->Is("GameObject"));
			Assert::IsTrue(rtti->Is("Attributed"));
			Assert::IsTrue(rtti->Is("Scope"));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(GameObject::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(GameObject::TypeIdClass(), rtti->TypeIdInstance());

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			GameObject* f = rtti->As<GameObject>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&gameObject, f);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&gameObject), fAsAttributed);

			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(GameObject::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsNotNull(r);

			Scope otherObject;
			Assert::IsFalse(rtti->Equals(&otherObject));

			Assert::AreEqual(std::string("GameObject"), r->ToString());
		}

		TEST_METHOD(Clone)
		{
			GameObject gameObject;
			GameObject& clone = *(gameObject.Clone());
			Assert::IsTrue(gameObject.Equals(&clone));
			Assert::IsTrue(clone.Is("GameObject"));
			delete& clone;

			Monster monster;
			Monster& clone2 = *(monster.Clone());
			Assert::IsTrue(monster.Equals(&clone2));
			Assert::IsTrue(monster.Is("Monster"));
			delete& clone2;
		}

		TEST_METHOD(CopyAssignment)
		{
			GameObject gameObject;
			GameObject gameObject2;

			Datum* name = gameObject.Find("Name");
			name->Set("NewName");

			gameObject2 = gameObject;
			Datum* name2 = gameObject2.Find("Name");
			Assert::AreEqual(name2->FrontString(), std::string("NewName"));

		}
		TEST_METHOD(MoveConstruction)
		{
			GameObject gameObject;

			Datum* name = gameObject.Find("Name");
			name->Set("NewName");

			GameObject gameObject2 = std::move(gameObject);
			Datum* name2 = gameObject2.Find("Name");
			Assert::AreEqual(name2->FrontString(), std::string("NewName"));

		}

		TEST_METHOD(MoveAssignment)
		{
			GameObject gameObject;
			GameObject gameObject2;

			Datum* name = gameObject.Find("Name");
			name->Set("NewName");

			 gameObject2 = std::move(gameObject);
			Datum* name2 = gameObject2.Find("Name");
			Assert::AreEqual(name2->FrontString(), std::string("NewName"));

		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState GameObjectTests::_startMemState;
}