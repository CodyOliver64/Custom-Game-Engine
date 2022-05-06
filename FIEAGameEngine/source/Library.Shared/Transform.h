#pragma once

#include "Attributed.h"
#include "TypeRegistry.h"
#include "Factory.h"

namespace FIEAGameEngine
{
	class Transform final : public Attributed
	{
		RTTI_DECLARATIONS(Transform, Attributed)

	public:

		/// <summary>
		/// Constructor of the Transform, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		Transform();

		Transform(const Transform&) = default;
		Transform(Transform&&) noexcept = default;
		Transform& operator=(const Transform&) = default;
		Transform& operator=(Transform&&) noexcept = default;
		~Transform() = default;

		glm::vec4 Position;
		glm::vec4 Rotation;
		glm::vec4 Scale;

		
		
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<Transform*> Clone() const override;

		/// <summary>
		/// RTTI Equality function that will compare the objects
		/// </summary>
		/// <param name="rhs">right compare</param>
		/// <returns>true if equal</returns>
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;


		/// <summary>
		/// Signature function required for adherence to the Datum/Scope/Attributed system
		/// Will return a Vector of Signatures for each of the prescribed attributes of the class. Used to register classes into the TypeRegistry
		/// </summary>
		/// <returns></returns>
		static const Vector<Signature> Signatures();

	};

	ConcreteFactory(Transform, Scope)
}

