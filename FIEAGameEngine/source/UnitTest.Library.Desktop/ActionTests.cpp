#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "Monster.h"
#include "Hero.h"
#include "ActionIncrement.h"
#include "ActionIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "WorldManager.h"



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
	TEST_CLASS(ActionTests)
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

		
		TEST_METHOD(ActionDestroyActionTest)
		{
			const std::string filename = R"(Content\ActionDestroyJson.json)";

			JsonTableParseHelper::SharedData sharedData(new GameObject);

			JsonTableParseHelper scopeParser;

			JsonParseCoordinator parseManager(sharedData);
			parseManager.AddHelper(scopeParser);

			AttributedFooFactory fooFactory;
			ScopeFactory scopeFactory;
			GameObjectFactory gameObjectFactory;
			HeroFactory heroFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionDestroyActionFactory actionCreateFactory;


			WorldManager testState;

			parseManager.ParseFromFile(filename);

			Datum* children = sharedData._rootScope->Find("Children");
			Assert::IsNotNull(children);
			Hero* testHero = children->GetScope()->As<Hero>();
			Assert::IsNotNull(testHero);


			Datum* health = testHero->Find("Health");
			Assert::IsNotNull(health);
			Assert::AreEqual(health->FrontInt(), 75);

			GameObject* level = sharedData._rootScope->As<GameObject>();
			Assert::IsNotNull(level);

			Datum* actionsDatum = testHero->Find("Actions");
			Assert::IsNotNull(actionsDatum);
			Assert::AreEqual(size_t(2), actionsDatum->Size());

			level->Update(testState);
			testState.updateActions();

			Assert::AreEqual(size_t(1), actionsDatum->Size());
			Assert::AreEqual(health->FrontInt(), 80);

			level->Update(testState);
			testState.updateActions();

			Assert::AreEqual(health->FrontInt(), 80);
		}
		
		TEST_METHOD(ActionCreateActionTest)
		{
			const std::string filename = R"(Content\ActionCreateJson.json)";

			JsonTableParseHelper::SharedData sharedData(new GameObject);

			JsonTableParseHelper scopeParser;

			JsonParseCoordinator parseManager(sharedData);
			parseManager.AddHelper(scopeParser);

			AttributedFooFactory fooFactory;
			ScopeFactory scopeFactory;
			GameObjectFactory gameObjectFactory;
			HeroFactory heroFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionCreateActionFactory actionCreateFactory;
			ActionListFactory actionIfFactory;


			WorldManager testState;

			parseManager.ParseFromFile(filename);

			Datum* children = sharedData._rootScope->Find("Children");
			Assert::IsNotNull(children);
			Hero* testHero = children->GetScope()->As<Hero>();
			Assert::IsNotNull(testHero);


			Datum* health = testHero->Find("Health");
			Assert::IsNotNull(health);
			Assert::AreEqual(health->FrontInt(), 75);

			GameObject* level = sharedData._rootScope->As<GameObject>();
			Assert::IsNotNull(level);

			Datum* actionsDatum = testHero->Find("Actions");
			Assert::IsNotNull(actionsDatum);
			Assert::AreEqual(size_t(1), actionsDatum->Size());

			level->Update(testState);
			testState.updateActions();

			Assert::AreEqual(size_t(2), actionsDatum->Size());

			Datum* targetDatum = actionsDatum->GetScope(1)->Find("Target");
			targetDatum->Set("Health");

			Assert::AreEqual(health->FrontInt(), 75);

			level->Update(testState);
			testState.updateActions();

			Assert::AreEqual(health->FrontInt(), 76);
		}
		
		TEST_METHOD(ActionIfTest)
		{
			const std::string filename = R"(Content\ActionJson.json)";

			JsonTableParseHelper::SharedData sharedData(new GameObject);

			JsonTableParseHelper scopeParser;

			JsonParseCoordinator parseManager(sharedData);
			parseManager.AddHelper(scopeParser);

			AttributedFooFactory fooFactory;
			ScopeFactory scopeFactory;
			GameObjectFactory gameObjectFactory;
			HeroFactory heroFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionIfFactory actionIfFactory;

			WorldManager testState;

			parseManager.ParseFromFile(filename);

			Datum* children = sharedData._rootScope->Find("Children");
			Assert::IsNotNull(children);
			Hero* testHero = children->GetScope()->As<Hero>();
			Assert::IsNotNull(testHero);


			Datum* health = testHero->Find("Health");
			Assert::IsNotNull(health);
			Assert::AreEqual(health->FrontInt(), 75);

			GameObject* level = sharedData._rootScope->As<GameObject>();
			Assert::IsNotNull(level);

			Datum* conditionDatum = level->Find("Children")->GetScope()->Find("Actions")->GetScope()->Find("Condition");
			Assert::IsNotNull(conditionDatum);

			level->Update(testState);
			Assert::AreEqual(health->FrontInt(), 80);

			conditionDatum->Set(1);

			level->Update(testState);
			Assert::AreEqual(health->FrontInt(), 75);

			level->Update(testState);
			Assert::AreEqual(health->FrontInt(), 70);
		}
		
		TEST_METHOD(ActionListTest)
		{
			ScopeFactory scopeFactory;
			GameObjectFactory gameObjectFactory;
			HeroFactory heroFactory;
			ActionDestroyActionFactory actionIncrementFactory;

			ActionList actionList;

			Datum* actionsDatum = actionList.Find("Actions");
			Assert::IsNotNull(actionsDatum);

			Assert::AreEqual(size_t(0), actionsDatum->Size());

			actionList.CreateAction("ActionDestroyAction", "");

			Assert::AreEqual(size_t(1), actionsDatum->Size());

			WorldManager testState;
			actionList.Update(testState);
			testState.updateActions();

			Assert::AreEqual(size_t(0), actionsDatum->Size());

		}

		TEST_METHOD(CloneTest)
		{
			ScopeFactory scopeFactory;
			GameObjectFactory gameObjectFactory;
			HeroFactory heroFactory;
			ActionDestroyActionFactory actionIncrementFactory;
			ActionCreateActionFactory createFactory;
			ActionIfFactory IfFactory;
			ActionIncrementFactory IncFactory;

			GameObject testObj;
			const Datum& actions = testObj.GetActions();
			const Datum& children = testObj.GetChildren();

			Assert::AreEqual(size_t(0), actions.Size());
			Assert::AreEqual(size_t(0), children.Size());

			{
				ActionList actionList;
				actionList.SetName("TestActionList");

				ActionList* cloneList = actionList.Clone();

				Assert::IsTrue(actionList.Equals(cloneList));
				Assert::IsFalse(actionList.Equals(&testObj));
				Assert::AreEqual(std::string("ActionList"), actionList.ToString());

				delete cloneList;
			}
			{
				ActionIf actionList;
				actionList.SetName("TestActionList");

				ActionIf* cloneList = actionList.Clone();

				Assert::IsTrue(actionList.Equals(cloneList));
				Assert::IsFalse(actionList.Equals(&testObj));
				Assert::AreEqual(std::string("ActionIf"), actionList.ToString());

				delete cloneList;
			}
			{
				ActionIncrement actionList;
				actionList.SetName("TestActionList");

				ActionIncrement* cloneList = actionList.Clone();

				Assert::IsTrue(actionList.Equals(cloneList));
				Assert::IsFalse(actionList.Equals(&testObj));
				Assert::AreEqual(std::string("ActionIncrement"), actionList.ToString());

				delete cloneList;
			}
			{
				ActionCreateAction actionList;
				actionList.SetName("TestActionList");

				ActionCreateAction* cloneList = actionList.Clone();

				Assert::IsTrue(actionList.Equals(cloneList));
				Assert::IsFalse(actionList.Equals(&testObj));
				Assert::AreEqual(std::string("ActionCreateAction"), actionList.ToString());

				delete cloneList;
			}
			{
				ActionDestroyAction actionList;
				actionList.SetName("TestActionList");

				ActionDestroyAction* cloneList = actionList.Clone();

				Assert::IsTrue(actionList.Equals(cloneList));
				Assert::IsFalse(actionList.Equals(&testObj));
				Assert::AreEqual(std::string("ActionDestroyAction"), actionList.ToString());

				delete cloneList;
			}
			{
				ActionList actionList;

				actionList.CreateAction("ActionIf", "Test1");
				actionList.CreateAction("ActionIncrement", "Test2");
				actionList.CreateAction("ActionDestroyAction", "Test3");
				actionList.CreateAction("ActionCreateAction", "Test4");

				ActionList list2 = actionList;

				Assert::IsTrue(list2.Equals(&actionList));
			}
		}
		
		TEST_METHOD(CreateActionTest)
		{
			Hero testHero;
			WorldManager testState;

			ScopeFactory scopeFactory;
			GameObjectFactory gameObjectFactory;
			HeroFactory monsterFactory;
			ActionIncrementFactory actionFactory;

			testHero.CreateAction("ActionIncrement", "A_IncreaseHealth");
			testHero.Find("Actions")->GetScope()->Find("Target")->Set("Health");

			Datum* health = testHero.Find("Health");
			Assert::AreEqual(health->FrontInt(), 100);

			testHero.Update(testState);

			Assert::AreEqual(health->FrontInt(), 101);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ActionTests::_startMemState;
}