#pragma once

#include "GameObject.h"

namespace FIEAGameEngine
{
	class Monster : public GameObject
	{
		RTTI_DECLARATIONS(Monster, GameObject)

	public:

		/// <summary>
		/// Constructor of the gameObject, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		Monster();

		/// <summary>
		/// Constructor of the gameObject that can be used by sub classes that inherit from Monster to pass along the class RTTI type ID
		/// </summary>
		Monster(size_t typeID);

		Monster(const Monster&) = default;
		Monster(Monster&&) noexcept = default;
		Monster& operator=(const Monster&) = default;
		Monster& operator=(Monster&&) noexcept = default;
		virtual ~Monster() = default;

		int Health{ 100 };

		virtual void Update(WorldManager& worldState) override;

	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<Monster*> Clone() const override;

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

	ConcreteFactory(Monster, Scope)
}




