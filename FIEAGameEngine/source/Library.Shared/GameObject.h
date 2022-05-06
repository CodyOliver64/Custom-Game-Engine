#pragma once

#include "Attributed.h"
#include "TypeRegistry.h"
#include "Factory.h"
#include "Transform.h"
#include "GameTime.h"
#include "GameClock.h"
#include "Action.h"
#include "WorldManager.h"

namespace FIEAGameEngine
{
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed)

	public:

		/// <summary>
		/// Constructor of the gameObject, this will call the Attributed constructor passing in the RTTI type id
		/// </summary>
		GameObject();

		/// <summary>
		/// Constructor of the gameObject that can be used by sub classes that inherit from GameObject to pass along the class RTTI type ID
		/// </summary>
		GameObject(size_t typeID);

		/// <summary>
		/// Copy Constructor for GameObject. Custom behavior needed to allocate new Transforms stored as external nested scope
		/// </summary>
		/// <param name=""></param>
		GameObject(const GameObject&);
		
		/// <summary>
		/// Copy Assignment for GameObject. Custom behavior needed to allocate new Transforms stored as external nested scope
		/// </summary>
		/// <param name="">GameObject reference to be copied from</param>
		/// <returns>Reference to the copied game Object</returns>
		GameObject& operator=(const GameObject&);

		GameObject(GameObject&&) noexcept;
		GameObject& operator=(GameObject&&) noexcept;

		/// <summary>
		/// Virtual destructor for GameObject. Used to free the memory allocated to store the external nested scope
		/// </summary>
		virtual ~GameObject();

		/// <summary>
		/// Main Update loop for all classes that derive from GameObject. Can be overwritten to add custom behavior to the update of an object
		/// Will assert that the objects in the children list are GameObjects (all children MUST be GameObjects).
		/// When a derived object overrides the Update method, it should call the base classes Update to ensure all children are updated as well
		/// </summary>
		/// <param name="gameTime">Takes in a gametime object that can be passed on and used in frame based calculations</param>
		virtual void Update(WorldManager& worldState);

		const Datum& GetActions() const;
		const Datum& GetChildren() const;

		void CreateAction(const std::string className, const std::string instanceName);
		
		Transform* transform;
		std::string objName;

	protected:
		Datum& _children{ At("Children") };
		Datum& _actions{ At("Actions") };

	
	
	public:
		//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns>newly constructed pointer</returns>
		virtual gsl::owner<GameObject*> Clone() const override;

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

	ConcreteFactory(GameObject, Scope)
}


