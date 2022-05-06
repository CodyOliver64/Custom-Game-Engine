#pragma once

#include "AttributedFoo.h"
#include "TypeRegistry.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Test class used to test the Datum/Scope/Attributed system
	/// </summary>
	class AttributedBar final : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedBar, AttributedFoo)

	public:
		static const std::size_t _arraySize = 7;

		/// <summary>
		/// Constructor of the test class, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		AttributedBar();

		AttributedBar(const AttributedBar&) = default;
		AttributedBar(AttributedBar&&) = default;
		AttributedBar& operator=(const AttributedBar&) = default;
		AttributedBar& operator=(AttributedBar&&) = default;
		~AttributedBar() = default;

		int SuperExternalInteger = 0;

		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<AttributedBar*> Clone() const override;

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
