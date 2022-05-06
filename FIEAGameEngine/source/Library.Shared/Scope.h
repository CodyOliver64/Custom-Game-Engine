#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <functional>
#include <string>
#include "glm\glm.hpp"
#include "RTTI.h"
#include "Datum.h"
#include "HashMap.h"
#include "gsl/gsl"
#include "Factory.h"

#pragma warning (push)
#pragma warning (disable : 4201)
#include "glm/gtx/string_cast.hpp"
#pragma warning (pop)

namespace FIEAGameEngine
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

	public:

		/// <summary>
		/// Default Constructor that takes in a predefined size (default 11) and constructs a scope
		/// </summary>
		Scope(std::size_t capacity = 11);
		
		/// <summary>
		/// Copy Constructor for Scope. Creates a new Scope with its own members in memory that store equivalent data as the scope being copied from
		/// </summary>
		/// <param name="other">Takes in a const scope to be copied from</param>
		Scope(const Scope& other);

		// <summary>
		/// Move Constructor that reassigns other scope to the new one. Other Scope is NOT to be used after Move is called
		/// </summary>
		/// <param name="other">Other Scope to be removed</param>
		Scope(Scope&& other) noexcept;
		
		// <summary>
		/// Copy Assignment that reassigns other scope to the new one after clearing out the memory of the other
		/// </summary>
		/// <param name="other">Other Scope to be copied</param>
		Scope& operator=(const Scope& other);
		
		// <summary>
		/// Move Assignment that reassigns other scope to the new one after clearing out the memory of the other. Other Scope is NOT to be used after Move is called
		/// </summary>
		/// <param name="other">Other Scope to be moveed/param>
		Scope& operator=(Scope&& other);
		
		/// <summary>
		/// Virtual scope destructor that will free all memory allocated in the scope, including any nested scope children
		/// </summary>
		virtual ~Scope();
		
		/// <summary>
		/// Wrapper function that allows polymorphic construction of scope derived objects
		/// </summary>
		/// <returns></returns>
		virtual gsl::owner<Scope*> Clone() const;

		/// <summary>
		/// Takes a constant string and returns the address of a Datum. This should return the address of the Datum associated with the given name in this Scope, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="key">string key that matches the data being searched for</param>
		/// <returns>Datum pointer of the desired data</returns>
		Datum* Find(const std::string& key);
		
		/// <summary>
		/// Takes a constant string and returns the address of a Datum. This should return the address of the Datum associated with the given name in this Scope, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="key">string key that matches the data being searched for</param>
		/// <returns>Datum pointer of the desired data</returns>
		const Datum* Find(const std::string& key) const;
		
		
		/// <summary>
		/// takes a constant string and the address of a Scope double pointer variable (whose default value could be nullptr), and which returns the address of a Datum. 
		/// This should return the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="key">string key that matches the data being searched for</param>
		/// <param name="scopeFound">optional output parameter that will point the scope where the datum was found</param>
		/// <returns>datum pointer</returns>
		Datum* Search(const std::string& key, Scope*& scopeFound);
		
		/// <summary>
		/// takes a constant string and the address of a Scope double pointer variable (whose default value could be nullptr), and which returns the address of a Datum. 
		/// This should return the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="key">string key that matches the data being searched for</param>
		/// <param name="scopeFound">optional output parameter that will point the scope where the datum was found</param>
		/// <returns>datum pointer</returns>
		Datum* Search(const std::string& key);

		/// <summary>
		/// takes a constant string and the address of a Scope double pointer variable (whose default value could be nullptr), and which returns the address of a Datum. 
		/// This should return the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="key">string key that matches the data being searched for</param>
		/// <param name="scopeFound">optional output parameter that will point the scope where the datum was found</param>
		/// <returns>datum pointer</returns>
		const Datum* Search(const std::string& key, const Scope*& scopeFound) const;
		
		/// <summary>
		/// takes a constant string and the address of a Scope double pointer variable (whose default value could be nullptr), and which returns the address of a Datum. 
		/// This should return the address of the most-closely nested Datum associated with the given name in this Scope or its ancestors, if it exists, and nullptr otherwise.
		/// </summary>
		/// <param name="key">string key that matches the data being searched for</param>
		/// <param name="scopeFound">optional output parameter that will point the scope where the datum was found</param>
		/// <returns>datum pointer</returns>
		const Datum* Search(const std::string& key) const;

		/// <summary>
		/// takes a constant string and returns a reference to a Datum. This should return a reference to a Datum with the associated name. If it already exists, return that one, otherwise create one.
		/// </summary>
		/// <param name="key">key to be searched for when deciding to create a new datum</param>
		/// <returns>reference to the datum created</returns>
		Datum& Append(const std::string& key);
		
		/// <summary>
		/// takes a constant string and which wraps Append, for syntactic convenience. (see Append)
		/// </summary>
		/// <param name="key">key to be searched or appended</param>
		/// <returns>datum reference</returns>
		Datum& operator[](const std::string& key);
		
		/// <summary>
		///takes an unsigned integer and which returns a reference to a Datum at the given index. The index values correspond to the order in which items were appended.
		/// </summary>
		/// <param name="index">index in the ordered vector that correpsonds to a keyDatum pair</param>
		/// <returns>Datum reference to the found scope</returns>
		Datum& operator[](size_t index);

		/// <summary>
		/// Takes a constant string and returns a reference to Scope. This should return a reference to a Scope with the associated name.  
		/// If a Datum already exists at that key reuse it (and append to it a new Scope), otherwise create a new Datum.
		/// the created Scope shall refer to its parent, such that Search operates as specified.
		/// </summary>
		/// <param name="key"> entry in which to append the scope</param>
		/// <returns>reference to the newly created scope</returns>
		Scope& AppendScope(const std::string& key);

		/// <summary>
		/// takes a reference to a Scope (the child to adopt), a string (the name of key for the Datum to use for storing the child).
		/// This scope will now belong to the parent scope as a nested scope that will be deleted upon the deletion of its parent
		/// </summary>
		/// <param name="child">scope to adopt</param>
		/// <param name="key">entry for the new adoption</param>
		void Adopt(Scope& child, const std::string& key);

		/// <summary>
		/// Removes a scope from its parent hierarchy. The scope can then be attributed to a new parent scope
		/// </summary>
		void Orphan();

		/// <summary>
		/// Deletes all nested scopes within a parent scope
		/// </summary>
		void Clear();

		/// <summary>
		/// takes the constant address of a Scope and returns the Datum pointer and index at which the Scope was found.
		/// </summary>
		/// <param name="searchingFor">scope to find</param>
		/// <returns>refernece to datum where the scope is stored, as well as its index</returns>
		std::pair<Datum*, std::size_t> FindContainedScope(Scope* const searchingFor);

		/// <summary>
		/// returns the address of the Scope which contains this one.
		/// </summary>
		Scope* GetParent() const;

		/// <summary>
		/// takes a const reference to a Scope and which returns a bool indicating that the two Scope objects have matching contents.
		/// This will cause a chain comparison on any scopes childed to the original scope
		/// </summary>
		/// <param name="other">scope to be compared against</param>
		/// <returns>bool if they are equal or not</returns>
		bool operator==(const Scope& other) const;
		/// <summary>
		/// takes a const reference to a Scope and which returns a bool indicating that the two Scope objects have matching contents.
		/// This will cause a chain comparison on any scopes childed to the original scope
		/// </summary>
		/// <param name="other">scope to be compared against</param>
		/// <returns>bool if they are equal or not</returns>
		bool operator!=(const Scope& other) const;
		
		/// <summary>
		/// overloaded RTTI equality. Casts to a scope in order to call the comparison on nested scopes
		/// </summary>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns the current size of the ordered vector
		/// </summary>
		std::size_t vectorSize() const;
		/// <summary>
		/// returns the current size of the KeyDatum hash map
		/// </summary>
		std::size_t hashMapSize() const;

		/// <summary>
		/// Returns a bool if the passed scope is a direct ancestor of the current scope
		/// </summary>
		/// /// <param name="scope">scope to check for ancestory</param>
		bool IsAncestorOf(const Scope& scope) const;

		/// <summary>
		/// Returns a bool if the passed scope is a direct descendant of the current scope
		/// </summary>
		/// /// <param name="scope">scope to check for ancestory</param>
		bool IsDescendantOf(const Scope& scope) const;


		Datum& At(const std::string key);
		const Datum& At(const std::string key) const;

		bool _isExternalScope{ false };


	protected:  
		Vector<typename HashMap<std::string, Datum>::PairType*> _vector;
		HashMap<std::string, Datum> _map;
		Scope* _parent{ nullptr };
	};

	ConcreteFactory(Scope, Scope)
}
