#include "pch.h"
#include "ActionIf.h"
#include <assert.h>


namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionIf)

	ActionIf::ActionIf() :
		ActionList(ActionIf::TypeIdClass())
	{

	}

	void ActionIf::Update(WorldManager& worldState)
	{
		assert(_actions.GetScope(condition)->Is(Action::TypeIdClass()));
		_actions.GetScope(condition)->As<Action>()->Update(worldState);
	}

	const Vector<Signature> ActionIf::Signatures()
	{
		return Vector<Signature>
		{
			{ "Condition", DatumTypes::Integer, 1, offsetof(ActionIf, condition) }
		};
	}

	gsl::owner<ActionIf*> ActionIf::Clone() const
	{
		return new ActionIf(*this);
	}

	std::string ActionIf::ToString() const
	{
		return "ActionIf";
	}

	bool ActionIf::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<ActionIf>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return actionName == rhsAttFoo->actionName;
	}
}