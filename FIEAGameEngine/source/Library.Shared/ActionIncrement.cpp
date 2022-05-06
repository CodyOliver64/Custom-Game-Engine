#include "pch.h"
#include "ActionIncrement.h"
#include <assert.h>


namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement)

	ActionIncrement::ActionIncrement() :
		Action(ActionIncrement::TypeIdClass())
	{
	}

	void ActionIncrement::Update(WorldManager& worldState)
	{
		worldState;
		if (targetDatum == nullptr)
		{
			targetDatum = Search(target);
			assert(targetDatum != nullptr && targetDatum->Type() == DatumTypes::Integer);
		}

		targetDatum->FrontInt() += step;
	}

	const Vector<Signature> ActionIncrement::Signatures()
	{
		return Vector<Signature>
		{
			{ "Target", DatumTypes::String, 1, offsetof(ActionIncrement, target) },
			{ "Step", DatumTypes::Integer, 1, offsetof(ActionIncrement, step) }
		};
	}

	gsl::owner<ActionIncrement*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

	std::string ActionIncrement::ToString() const
	{
		return "ActionIncrement";
	}

	bool ActionIncrement::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<ActionIncrement>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return actionName == rhsAttFoo->actionName;
	}
}


