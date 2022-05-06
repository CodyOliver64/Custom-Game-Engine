#pragma once
#include "HashMap.h"
#include "Signature.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// TypeRegistry is a singleton class with only one static instance. This registry is built pre-main using the Signatures of any classes that adhere to the Datum/Scope/Attributed storage system
	/// Classes can then make calls to the registry on construction to dynamically build their matching scope
	/// </summary>
	class TypeRegistry final
	{

	public:
		
		/// <summary>
		/// Register type is a template that takes in the class being registered, and its parent (if it has one)
		/// It will then add a vector of attribute signatures to the registry (stored as a hash map), along with its Type ID.
		/// The registration supports inheritance and will combine signatures of itself and its parent, along the entire chain of inheritance
		/// </summary>
		/// <typeparam name="Base">Base class being registered</typeparam>
		/// <typeparam name="Parent">Parent of the base class (if applicable)</typeparam>
		template <typename Base, typename Parent>
		void RegisterType()
		{
			if (Parent::TypeIdClass() != Attributed::TypeIdClass())
			{
				_registry.Insert(std::make_pair(Base::TypeIdClass(), _registry.At(Parent::TypeIdClass()) + Base::Signatures()));
			}
			else
			{
				_registry.Insert(std::make_pair(Base::TypeIdClass(), Base::Signatures()));
			}
		}
		
		/// <summary>
		/// Given an RTTI type ID, this function will return a reference to the vector that hold all signature of the class type being passed by ID
		/// </summary>
		/// <param name="typeID">ID of the class whos signatures are being returned</param>
		/// <returns>reference to the vector that holds the classes attributes</returns>
		const Vector<Signature>& GetSignatures(size_t typeID);

		/// <summary>
		/// Singleton GetInstance that will return that one and only static instance of the Registry. Will create the instance if it has not yet been created.
		/// Functions will then be called from this instance
		/// </summary>
		/// <returns></returns>
		static TypeRegistry* GetInstance();

	private:
		static TypeRegistry* _typeRegistryInstance;
		HashMap<std::size_t, Vector<Signature>> _registry;
	};
}
