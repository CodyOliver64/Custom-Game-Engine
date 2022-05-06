#pragma once

#include "Attributed.h"
#include "GameTime.h"
#include "GameClock.h"
#include "WorldManager.h"

namespace FIEAGameEngine
{
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:

		/// <summary>
		/// Constructor of the gameObject that can be used by sub classes that inherit from Action to pass along the class RTTI type ID
		/// </summary>
		Action(size_t typeID);

		Action(const Action&) = default;
		Action& operator=(const Action&) = default;
		Action(Action&&) noexcept = default;
		Action& operator=(Action&&) noexcept = default;

		/// <summary>
		/// Virtual destructor for Action
		/// </summary>
		virtual ~Action() = default;

		/// <summary>
		/// Main Update loop for all classes that derive from Action. Can be overwritten to add custom behavior to the update of an action
		/// </summary>
		/// <param name="gameTime">Takes in a gametime object that can be passed on and used in frame based calculations</param>
		virtual void Update(WorldManager& worldState) = 0;

		/// <summary>
		/// Sets the current name of the Action as a string
		/// </summary>
		/// <param name="newName">new name to be set</param>
		void SetName(const std::string& newName);
		/// <summary>
		/// Returns the current name of the Action as a string
		/// </summary>
		const std::string& GetName() const;


		/// <summary>
		/// Signature function required for adherence to the Datum/Scope/Attributed system
		/// Will return a Vector of Signatures for each of the prescribed attributes of the class. Used to register classes into the TypeRegistry
		/// </summary>
		/// <returns></returns>
		static const Vector<Signature> Signatures();


		std::string actionName;

	};
}

