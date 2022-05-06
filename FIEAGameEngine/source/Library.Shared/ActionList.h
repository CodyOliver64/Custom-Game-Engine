#pragma once

#include "Action.h"
#include "Factory.h"
#include "GameTime.h"
#include "GameClock.h"

namespace FIEAGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:

		/// <summary>
		/// Constructor of the gameObject, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		ActionList();

		/// <summary>
		/// Constructor of the gameObject that can be used by sub classes that inherit from ActionList to pass along the class RTTI type ID
		/// </summary>
		ActionList(size_t typeID);

		
		ActionList(const ActionList&) = default;
		ActionList& operator=(const ActionList&) = default;
		ActionList(ActionList&&) noexcept = default;
		ActionList& operator=(ActionList&&) noexcept = default;
		virtual ~ActionList() = default;

		/// <summary>
		/// Main Update loop for all classes that derive from ActionList. Can be overwritten to add custom behavior to the update of an object
		/// Will assert that the objects in the children list are Actions (all children MUST be Actions).
		/// When a derived object overrides the Update method, it should call the base classes Update to ensure all children are updated as well
		/// </summary>
		/// <param name="gameTime">Takes in a gametime object that can be passed on and used in frame based calculations</param>
		virtual void Update(WorldManager& worldState) override;

		/// <summary>
		/// Helper function that allows a ActionList object to create new actions and add them to their Actions Datum
		/// </summary>
		/// <param name="className">class name to be created as a string</param>
		/// <param name="instanceName">instance name of the created class as a string</param>
		void CreateAction(const std::string className, const std::string instanceName);

		Datum& _actions{ At("Actions") };

	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		virtual gsl::owner<ActionList*> Clone() const override;

		/// <summary>
		/// RTTI Equality function that will compare the objects
		/// </summary>
		/// <param name="rhs">right compare</param>
		/// <returns>true if equal</returns>
		virtual bool Equals(const RTTI* rhs) const override;
		virtual std::string ToString() const override;


		/// <summary>
		/// Signature function required for adherence to the Datum/Scope/Attributed system
		/// Will return a Vector of Signatures for each of the prescribed attributes of the class. Used to register classes into the TypeRegistry
		/// </summary>
		/// <returns></returns>
		static const Vector<Signature> Signatures();

	};

	ConcreteFactory(ActionList, Scope)
}




