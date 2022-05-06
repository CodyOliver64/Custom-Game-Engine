#pragma once
#include "Scope.h"
#include "TypeRegistry.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Attributed is a class that wraps the Datum/Scope solution and dynamically builds scope objects that match the attributes of a class that derives from Attributed
	/// This class is abstract and is meant to be inherited from by objects that wish to subscribe to the gameObject system that is used to expose a classes members to the engines scripting language
	/// </summary>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)

	public:
		/// <summary>
		/// Defaulted Destructor, calls destructor on members that will ensure memory is freed
		/// </summary>
		virtual ~Attributed() = default; 
		
		/// <summary>
		/// Copy Constructor for Attributed. Creates a new Attributed object using Scopes copy constructor to give an equivalent scope with its own memory
		/// Repopulate will then update any external memory to point to the new location of the copied object
		/// </summary>
		/// <param name="other">Takes in a const Attributed object to be copied from</param>
		Attributed(const Attributed& other);

		/// <summary>
		/// Copy Assignment for Attributed. Copies the right hand side to the left hand side using Scopes copy assignment to give an equivalent scope with its own memory
		/// Repopulate will then update any external memory to point to the new location of the copied object
		/// </summary>
		/// <param name="other">Takes in a const Attributed object to be copied from</param>
		Attributed& operator=(const Attributed& other);

		/// <summary>
		/// Move Constructor for Attributed. Creates a new Attributed object using Scopes move constructor to give an equivalent scope, the moved Attributed object is NOT to be used after move
		/// Repopulate will then update any external memory to point to the new location of the copied object
		/// </summary>
		/// <param name="other">Takes in a const Attributed object to be moved</param>
		Attributed(Attributed&& other);
		
		/// <summary>
		/// Move Assignment for Attributed. Copies the right hand side to the left hand side using Scopes move assignment to give an equivalent scope, the moved Attributed object is NOT to be used after move
		/// Repopulate will then update any external memory to point to the new location of the copied object
		/// </summary>
		/// <param name="other">Takes in a const Attributed object to be moved</param>
		Attributed& operator=(Attributed&& other);

		/// <summary>
		/// Given a string (which must not be the name of a prescribed attribute), append a Datum to this Scope with the given name, and return a reference to that Datum. If the Datum already existed, return it.
		/// Append wrapper
		/// </summary>
		/// <param name="key">string key to be appended</param>
		/// <returns>datum reference of the appended datum</returns>
		Datum& AppendAuxiliaryAttribute(const std::string& key);

		/// <summary>
		/// Given a string, return a bool indicating whether it is the name of an attribute for this object.
		/// </summary>
		/// <param name="key">string key to be searched and analyzed</param>
		/// <returns>True or False, true if the key is an attribute</returns>
		bool IsAttribute(const std::string& key);

		/// <summary>
		/// Given a string, return a bool indicating whether it is the name of an Prescribed attribute for this object.
		/// A Prescribed attribute is any attribute that belongs to every instance of the class
		/// </summary>
		/// <param name="key">string key to be searched and analyzed</param>
		/// <returns>True or False, true if the key is an attribute</returns>
		bool IsPrescribedAttribute(const std::string& key);
		
		/// <summary>
		/// Given a string, return a bool indicating whether it is the name of an Auxiliary attribute for this object.
		/// An Auxiliary attribute is any attribute that is not Prescribed (meaning it has been added beyond what is required for the class)
		/// </summary>
		/// <param name="key">string key to be searched and analyzed</param>
		/// <returns>True or False, true if the key is an attribute</returns>
		bool IsAuxiliaryAttribute(const std::string& key);

		
		//Will add Getters later after researching std::for_each functor
		//Vector<const std::string&> GetAttributes();
		//Vector<const std::string&> GetPrescribedAttributes();
		//Vector<const std::string&> GetAuxiliaryAttributes();


	protected:
		
		/// <summary>
		/// Constructor of Attributed that takes in an RTTI Type ID that is used to Populate() a Scope that matches the Attributed object being constructed
		/// </summary>
		/// <param name="typeID">RTTI Type ID that matches the assigned ID for the class being constructed</param>
		Attributed(size_t typeID);
		
		/// <summary>
		/// Appends each prescribed String/Datum pair of the appropriate type inside the Scope associated (is-a relationship) with this Attributed object
		/// This will also set the external storage of the created datum to point to the objects members in memory. May throw an exception if types are registered incorrectly
		/// </summary>
		/// <param name="typeID">RTTI Type ID that matches the assigned ID for the class being constructed</param>
		void Populate(size_t typeID);

		/// <summary>
		/// On copy/move, this function will update all external storage pointers within the copied scope so that the external storage points to the new location
		/// </summary>
		/// <param name="typeID">RTTI Type ID that matches the assigned ID for the class being constructed</param>
		void Repopulate(size_t typeID);

		size_t _numPrescribedAttributes;
	};
}
