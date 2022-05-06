#pragma once

#include "Action.h"
#include "Factory.h"
#include "GameTime.h"
#include "GameClock.h"

namespace FIEAGameEngine
{
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:

		/// <summary>
		/// Constructor of the gameObject, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		ActionIncrement();


		ActionIncrement(const ActionIncrement&) = default;
		ActionIncrement& operator=(const ActionIncrement&) = default;
		ActionIncrement(ActionIncrement&&) noexcept = default;
		ActionIncrement& operator=(ActionIncrement&&) noexcept = default;
		~ActionIncrement() = default;

		/// <summary>
		/// Main Update loop for all classes that derive from ActionIncrement. Can be overwritten to add custom behavior to the update of an object
		/// Will assert that the objects in the children list are ActionIncrements (all children MUST be ActionIncrements).
		/// When a derived object overrides the Update method, it should call the base classes Update to ensure all children are updated as well
		/// </summary>
		/// <param name="gameTime">Takes in a gametime object that can be passed on and used in frame based calculations</param>
		void Update(WorldManager& worldState) override;

		std::string target;
		std::int32_t step{ 1 };

		Datum* targetDatum{ nullptr };

	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<ActionIncrement*> Clone() const override;

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

	ConcreteFactory(ActionIncrement, Scope)
}

