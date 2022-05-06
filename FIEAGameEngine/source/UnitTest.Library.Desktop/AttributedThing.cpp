#include "pch.h"
#include "AttributedThing.h"



namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(AttributedThing)

		AttributedThing::AttributedThing() :
		AttributedFoo(AttributedThing::TypeIdClass())
	{
	}

	const Vector<Signature> AttributedThing::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", DatumTypes::Float, 1, offsetof(AttributedThing, ExternalInteger) }
		};
	}


	gsl::owner<AttributedThing*> AttributedThing::Clone() const
	{
		return new AttributedThing(*this);
	}

	std::string AttributedThing::ToString() const
	{
		return "AttributedThing";
	}

	bool AttributedThing::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<AttributedThing>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return ExternalInteger == rhsAttFoo->ExternalInteger &&
			ExternalFloat == rhsAttFoo->ExternalFloat &&
			ExternalString == rhsAttFoo->ExternalString &&
			ExternalVector == rhsAttFoo->ExternalVector &&
			ExternalMatrix == rhsAttFoo->ExternalMatrix &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray)) &&
			(*this).At("NestedScope") == (*rhsAttFoo).At("NestedScope");
	}
}