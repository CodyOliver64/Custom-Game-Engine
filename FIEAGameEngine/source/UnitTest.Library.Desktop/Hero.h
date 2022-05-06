#pragma once

#include "GameObject.h"

namespace FIEAGameEngine
{
	class Hero : public GameObject
	{
		RTTI_DECLARATIONS(Hero, GameObject)

	public:

		/// <summary>
		/// Constructor of the gameObject, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		Hero();

		/// <summary>
		/// Constructor of the gameObject that can be used by sub classes that inherit from Hero to pass along the class RTTI type ID
		/// </summary>
		Hero(size_t typeID);

		Hero(const Hero&) = default;
		Hero(Hero&&) noexcept = default;
		Hero& operator=(const Hero&) = default;
		Hero& operator=(Hero&&) noexcept = default;
		virtual ~Hero() = default;

		int Health{ 100 };

		virtual void Update(WorldManager& worldState) override;

	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<Hero*> Clone() const override;

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

	ConcreteFactory(Hero, Scope)
}





