#pragma once

#include "ActionList.h"
#include "Factory.h"
#include "GameTime.h"
#include "GameClock.h"

namespace FIEAGameEngine
{
	class ActionIf final : public ActionList
	{
		RTTI_DECLARATIONS(ActionIf, ActionList)

	public:

		/// <summary>
		/// Constructor of the gameObject, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		ActionIf();


		ActionIf(const ActionIf&) = default;
		ActionIf& operator=(const ActionIf&) = default;
		ActionIf(ActionIf&&) noexcept = default;
		ActionIf& operator=(ActionIf&&) noexcept = default;
		~ActionIf() = default;

		/// <summary>
		/// Main Update loop for all classes that derive from ActionIf. Can be overwritten to add custom behavior to the update of an object
		/// Will assert that the objects in the children list are ActionIfs (all children MUST be ActionIfs).
		/// When a derived object overrides the Update method, it should call the base classes Update to ensure all children are updated as well
		/// </summary>
		/// <param name="gameTime">Takes in a gametime object that can be passed on and used in frame based calculations</param>
		void Update(WorldManager& worldState) override;

		/// <summary>
		/// Condition which is used to call Update on the list of actions (index 0 or 1)
		/// 0 = false, 1 = true
		/// </summary>
		size_t condition{ 0 };

	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		gsl::owner<ActionIf*> Clone() const override;

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

	ConcreteFactory(ActionIf, Scope)
}



