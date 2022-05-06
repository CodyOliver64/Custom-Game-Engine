#pragma once

#include "AttributedFoo.h"
#include "TypeRegistry.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Test class used to test the Datum/Scope/Attributed system
	/// </summary>
	class AttributedThing final : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedThing, AttributedFoo)

	public:
		static const std::size_t _arraySize = 7;

		/// <summary>
		/// Constructor of the test class, this will call the Attributed constructor passing in the RTTI type id
		/// This class should not be instantiated, it is only meant to test an edge case where an inherited class has an attribute with the same name, but different types
		/// </summary>
		AttributedThing();

		AttributedThing(const AttributedThing&) = default;
		AttributedThing(AttributedThing&&) = default;
		AttributedThing& operator=(const AttributedThing&) = default;
		AttributedThing& operator=(AttributedThing&&) = default;
		~AttributedThing() = default;

		float ExternalInteger = 0;

		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<AttributedThing*> Clone() const override;
		
		/// <summary>
		/// RTTI Equality function that will compare the objects
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;

		/// <summary>
		/// Signature function required for adherence to the Datum/Scope/Attributed system
		/// Will return a Vector of Signatures for each of the prescribed attributes of the class. Used to register classes into the TypeRegistry
		/// </summary>
		/// <returns></returns>
		static const Vector<Signature> Signatures();

	};
}
