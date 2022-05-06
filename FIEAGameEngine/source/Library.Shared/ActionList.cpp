#include "pch.h"
#include "ActionList.h"
#include <assert.h>


namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList() :
		Action(ActionList::TypeIdClass())
	{
		
	}

	ActionList::ActionList(size_t typeID) :
		Action(typeID)
	{
	}

	void ActionList::Update(WorldManager& worldState)
	{
		//Update Actions
		for (size_t i = 0; i < _actions.Size(); ++i)
		{
			assert(_actions.GetScope(i)->Is(Action::TypeIdClass()));
			_actions.GetScope(i)->As<Action>()->Update(worldState);
		}
	}

	void ActionList::CreateAction(const std::string className, const std::string instanceName)
	{
		Action* newAction = Factory<Scope>::Create(className)->As<Action>();
		assert(newAction->Is(Action::TypeIdClass()));
		newAction->SetName(instanceName);
		Adopt(*newAction, "Actions");
	}


	const Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{ "Actions", DatumTypes::Table, 0,  0 }
		};
	}

	gsl::owner<ActionList*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	std::string ActionList::ToString() const
	{
		return "ActionList";
	}

	bool ActionList::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<ActionList>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return actionName == rhsAttFoo->actionName;
	}
}

