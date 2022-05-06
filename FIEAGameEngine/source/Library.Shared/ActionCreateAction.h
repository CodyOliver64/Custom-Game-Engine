#pragma once

#include "Action.h"
#include "Factory.h"
#include "GameTime.h"
#include "GameClock.h"
#include "WorldManager.h"
#include "GameObject.h"

namespace FIEAGameEngine
{
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:

		/// <summary>
		/// Constructor of the gameObject, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		ActionCreateAction();

		ActionCreateAction(const ActionCreateAction&) = default;
		ActionCreateAction& operator=(const ActionCreateAction&) = default;
		ActionCreateAction(ActionCreateAction&&) noexcept = default;
		ActionCreateAction& operator=(ActionCreateAction&&) noexcept = default;
		~ActionCreateAction() = default;

		/// <summary>
		/// The main update loop for ActionCreateAction will create a new action based on the prototype name and add them to the delayed list of actions to be added after update
		/// Requires a target name to find where the new actions should be adopted to
		/// </summary>
		/// <param name="gameTime">Takes in a gametime object that can be passed on and used in frame based calculations</param>
		void Update(WorldManager& worldState) override;

		std::string prototypeName;
		std::string actionName;
		std::string targetName;

		Scope* _target{ nullptr };
		bool _isFound{ false };

	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<ActionCreateAction*> Clone() const override;

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

	ConcreteFactory(ActionCreateAction, Scope)
}

