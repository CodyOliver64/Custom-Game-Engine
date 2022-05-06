#include "pch.h"
#include "ActionCreateAction.h"
#include <assert.h>


namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)

	ActionCreateAction::ActionCreateAction() :
		Action(ActionCreateAction::TypeIdClass())
	{

	}

	void ActionCreateAction::Update(WorldManager& worldState)
	{
		Scope* newAction = Factory<Scope>::Create(prototypeName);
		assert(newAction != nullptr);
		assert(newAction->As<Action>() != nullptr);

		static_cast<Action*>(newAction)->SetName(actionName);

		if (_target == nullptr)
		{
			//Find the target
			Scope* targetScope = this;
			Datum* searchDatum;
			while (targetScope->GetParent() != nullptr)
			{
				targetScope = targetScope->GetParent();

				searchDatum = targetScope->Find("Children");
				for (size_t i = 0; i < searchDatum->Size(); ++i)
				{
					if (searchDatum->GetScope(i)->As<GameObject>()->objName == targetName)
					{
						_target = searchDatum->GetScope(i);
						_isFound = true;
						worldState._pendingActionsCreated.PushBack(std::make_pair(newAction, _target));
						return;
					}
				}

				searchDatum = targetScope->Find("Actions");
				for (size_t i = 0; i < searchDatum->Size(); ++i)
				{
					if (searchDatum->GetScope(i)->As<Action>()->GetName() == targetName)
					{
						_target = searchDatum->GetScope(i);
						_isFound = true;
						worldState._pendingActionsCreated.PushBack(std::make_pair(newAction, _target));
						return;
					}
				}
			}
		}
		else if (_isFound)
		{
			worldState._pendingActionsCreated.PushBack(std::make_pair(newAction, _target));
		}
	}

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Prototype", DatumTypes::String, 1, offsetof(ActionCreateAction, prototypeName) },
			{ "ActionName", DatumTypes::String, 1, offsetof(ActionCreateAction, actionName) },
			{ "TargetName", DatumTypes::String, 1, offsetof(ActionCreateAction, targetName) }
		};
	}

	gsl::owner<ActionCreateAction*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}

	std::string ActionCreateAction::ToString() const
	{
		return "ActionCreateAction";
	}

	bool ActionCreateAction::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<ActionCreateAction>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return actionName == rhsAttFoo->actionName;
	}
}


