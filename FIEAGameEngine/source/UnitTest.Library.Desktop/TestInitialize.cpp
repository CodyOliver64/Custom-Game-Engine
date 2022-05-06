#include "pch.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "AttributedThing.h"
#include "TypeRegistry.h"
#include "GameObject.h"
#include "Monster.h"
#include "Hero.h"
#include "Transform.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "ActionIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		TypeRegistry::GetInstance()->RegisterType<AttributedFoo, Attributed>();
		TypeRegistry::GetInstance()->RegisterType<AttributedBar, AttributedFoo>();
		TypeRegistry::GetInstance()->RegisterType<AttributedThing, AttributedFoo>();

		
		
		TypeRegistry::GetInstance()->RegisterType<Transform, Attributed>();
		TypeRegistry::GetInstance()->RegisterType<GameObject, Attributed>();
		TypeRegistry::GetInstance()->RegisterType<Monster, GameObject>();
		TypeRegistry::GetInstance()->RegisterType<Hero, GameObject>();

		TypeRegistry::GetInstance()->RegisterType<Action, Attributed>();
		TypeRegistry::GetInstance()->RegisterType<ActionList, Action>();
		TypeRegistry::GetInstance()->RegisterType<ActionIncrement, Action>();
		TypeRegistry::GetInstance()->RegisterType<ActionIf, ActionList>();
		TypeRegistry::GetInstance()->RegisterType<ActionCreateAction, Action>();
		TypeRegistry::GetInstance()->RegisterType<ActionDestroyAction, Action>();
	}
}