#include "pch.h"
#include "Action.h"
#include <assert.h>


namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Action)


	Action::Action(size_t typeID) :
		Attributed(typeID)
	{
	}

	
	void Action::SetName(const std::string& newName)
	{
		actionName = newName;
	}
	const std::string& Action::GetName() const
	{
		return actionName;
	}
	
	const Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", DatumTypes::String, 1, offsetof(Action, actionName) }
		};
	}
}
