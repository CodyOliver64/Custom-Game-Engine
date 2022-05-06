#include "pch.h"
#include "Transform.h"



namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Transform)

	Transform::Transform() :
		Attributed(Transform::TypeIdClass())
	{
		_isExternalScope = true;
	}

	const Vector<Signature> Transform::Signatures()
	{
		return Vector<Signature>
		{
			{ "Position", DatumTypes::Vector, 1, offsetof(Transform, Position) },
			{ "Rotation", DatumTypes::Vector, 1, offsetof(Transform, Rotation) },
			{ "Scale", DatumTypes::Vector, 1, offsetof(Transform, Scale) }
		};
	}



	gsl::owner<Transform*> Transform::Clone() const
	{
		return new Transform(*this);
	}

	std::string Transform::ToString() const
	{
		return "Transform";
	}

	bool Transform::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<Transform>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return this == rhs;
	}
}

