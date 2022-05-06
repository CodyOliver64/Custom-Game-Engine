#include "pch.h"
#include "ActionDestroyAction.h"
#include <assert.h>


namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction)

	ActionDestroyAction::ActionDestroyAction() :
		Action(ActionDestroyAction::TypeIdClass())
	{

	}

	void ActionDestroyAction::Update(WorldManager& worldState)
	{
		Scope* targetScope = this;
		Datum* searchDatum;
		while (targetScope->GetParent() != nullptr)
		{
			targetScope = targetScope->GetParent();
			searchDatum = targetScope->Find("Actions");
			for (std::size_t i = 0; i < searchDatum->Size(); ++i)
			{
				Scope* target = searchDatum->GetScope(i);
				if (target->As<Action>()->GetName() == actionToDelete)
				{
					worldState._pendingActionsRemoved.PushBack(target);
					return;
				}
			}
		}
	}

	const Vector<Signature> ActionDestroyAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "ActionToDelete", DatumTypes::String, 1, offsetof(ActionDestroyAction, actionToDelete) }
		};
	}

	gsl::owner<ActionDestroyAction*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction(*this);
	}

	std::string ActionDestroyAction::ToString() const
	{
		return "ActionDestroyAction";
	}

	bool ActionDestroyAction::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<ActionDestroyAction>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return actionName == rhsAttFoo->actionName;
	}
}
