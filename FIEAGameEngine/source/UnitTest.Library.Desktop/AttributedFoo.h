#pragma once

#include "Attributed.h"
#include "TypeRegistry.h"
#include "Factory.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Test class used to test the Datum/Scope/Attributed system
	/// </summary>
	class AttributedFoo : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed)

	public:
		static const std::size_t _arraySize = 7;
		
		/// <summary>
		/// Constructor of the test class, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		AttributedFoo();

		AttributedFoo(const AttributedFoo&) = default;
		AttributedFoo(AttributedFoo&&) noexcept = default;
		AttributedFoo& operator=(const AttributedFoo&) = default;
		AttributedFoo& operator=(AttributedFoo&&) noexcept = default;
		~AttributedFoo() = default;

		int ExternalInteger = 0;
		float ExternalFloat = 0;
		std::string ExternalString;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArray[_arraySize];
		float ExternalFloatArray[_arraySize];
		std::string ExternalStringArray[_arraySize];
		glm::vec4 ExternalVectorArray[_arraySize];
		glm::mat4 ExternalMatrixArray[_arraySize];

		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<AttributedFoo*> Clone() const override;

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

		/// <summary>
		/// Constructor of the test class that can be used by sub classes that inherit from AttributedFoo to pass along the class RTTI type ID
		/// </summary>
		AttributedFoo(size_t typeID);
		
	};

	ConcreteFactory(AttributedFoo, Scope)
}
