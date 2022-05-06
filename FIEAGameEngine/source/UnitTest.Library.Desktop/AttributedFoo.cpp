#include "pch.h"
#include "AttributedFoo.h"



namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(AttributedFoo)
	
	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{
	}

	AttributedFoo::AttributedFoo(size_t typeID) :
		Attributed(typeID), ExternalVector{}, ExternalMatrix{}, ExternalIntegerArray{}, ExternalFloatArray{}, ExternalVectorArray{}, ExternalMatrixArray{}
	{
	}
	
	const Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", DatumTypes::Integer, 1, offsetof(AttributedFoo, ExternalInteger) },
			{ "ExternalFloat", DatumTypes::Float, 1, offsetof(AttributedFoo, ExternalFloat) },
			{ "ExternalString", DatumTypes::String, 1, offsetof(AttributedFoo, ExternalString) },
			{ "ExternalVector", DatumTypes::Vector, 1, offsetof(AttributedFoo, ExternalVector) },
			{ "ExternalMatrix", DatumTypes::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray", DatumTypes::Integer, _arraySize, offsetof(AttributedFoo, ExternalIntegerArray) },
			{ "ExternalFloatArray", DatumTypes::Float, _arraySize, offsetof(AttributedFoo, ExternalFloatArray) },
			{ "ExternalStringArray", DatumTypes::String, _arraySize, offsetof(AttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray", DatumTypes::Vector, _arraySize, offsetof(AttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray", DatumTypes::Matrix, _arraySize, offsetof(AttributedFoo, ExternalMatrixArray) },
		};
	}
	
	
	
	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo";
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<AttributedFoo>();

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
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttFoo->ExternalIntegerArray));
	}
}