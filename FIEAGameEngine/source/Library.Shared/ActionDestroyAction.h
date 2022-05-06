#pragma once

#include "Action.h"
#include "Factory.h"
#include "GameTime.h"
#include "GameClock.h"
#include "WorldManager.h"

namespace FIEAGameEngine
{
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:

		/// <summary>
		/// Constructor of the gameObject, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		ActionDestroyAction();


		ActionDestroyAction(const ActionDestroyAction&) = default;
		ActionDestroyAction& operator=(const ActionDestroyAction&) = default;
		ActionDestroyAction(ActionDestroyAction&&) noexcept = default;
		ActionDestroyAction& operator=(ActionDestroyAction&&) noexcept = default;
		~ActionDestroyAction() = default;

		/// <summary>
		/// The main update loop for ActionCreateAction will mark an action to be deleted based on its instance name, adding the action to the delayed list of action to be deleted
		/// </summary>
		/// <param name="gameTime">Takes in a gametime object that can be passed on and used in frame based calculations</param>
		void Update(WorldManager& worldState) override;

		std::string actionToDelete;

	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<ActionDestroyAction*> Clone() const override;

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

	ConcreteFactory(ActionDestroyAction, Scope)
}



