#include "pch.h"
#include "AttributedBar.h"



namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(AttributedBar)

		AttributedBar::AttributedBar() :
			AttributedFoo(AttributedBar::TypeIdClass())
	{
	}

	const Vector<Signature> AttributedBar::Signatures()
	{
		return Vector<Signature>
		{
			{ "SuperExternalInteger", DatumTypes::Integer, 1, offsetof(AttributedBar, SuperExternalInteger) }
		};
	}


	gsl::owner<AttributedBar*> AttributedBar::Clone() const
	{
		return new AttributedBar(*this);
	}

	std::string AttributedBar::ToString() const
	{
		return "AttributedBar";
	}

	bool AttributedBar::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<AttributedBar>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return SuperExternalInteger == rhsAttFoo->SuperExternalInteger &&
			ExternalInteger == rhsAttFoo->ExternalInteger &&
			ExternalFloat == rhsAttFoo->ExternalFloat &&
			ExternalString == rhsAttFoo->ExternalString &&
			ExternalVector == rhsAttFoo->ExternalVector &&
			ExternalMatrix == rhsAttFoo->ExternalMatrix &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray));
	}
}