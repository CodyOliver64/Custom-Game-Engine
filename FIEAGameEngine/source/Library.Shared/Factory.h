#pragma once
#include "HashMap.h"
#include "gsl/gsl"

namespace FIEAGameEngine
{
	/// <summary>
	/// Factory is a design pattern implemented using static methods within this header file that allows the creation of objects without foreknowledge of the identifying names of the object
	/// The class serves three main purposes:
	/// 1 - It is template, which allows the user to have "families" of factories, each with their own static container of of factories
	/// 2 - It is a manager of all factories, allowing the end user to call static functions to get proper pointers from the list of factories
	/// 3 - It is a template, allowing the use of the Concrete macro to define factory classes based on the objects each factory knows how to create
	/// 
	/// For each object type that the Factory will handle, a matching Concrete Macro call must be made to define the derived Factory class.
	/// These factories must be managed by the user, and must stay in scope throughout runtime.
	/// </summary>
	/// <typeparam name="T">Abstract class that is used to group families of factories</typeparam>
	template <typename T>
	class Factory
	{
	public:
		Factory() = default;
		virtual ~Factory() = default;

		//Deleted Move & Copy - Factory static memory should not be moved or copied
		Factory(const Factory<T>& other) = delete;
		Factory<T>& operator=(const Factory<T>& other) = delete;
		Factory(Factory<T>&& other) = delete;
		Factory<T>& operator=(Factory<T>&& other) = delete;

		/// <summary>
		/// Given a class name, return the associated concrete factory.
		/// </summary>
		/// <param name="className">string key that is the name of the class of the product of the factory being located</param>
		/// <returns>returns a pointer to the Factor, returns null if the factory is not found</returns>
		static const Factory<T>* Find(const std::string className);
		
		/// <summary>
		/// Given a class name, return a new object of that type.
		/// </summary>
		/// <param name="className">string key that is the name of the class that you wish to instantiate</param>
		/// <returns>Pointer to the newly created object (user owns this memory)</returns>
		static gsl::owner <T*> Create(const std::string className);

		/// <summary>
		/// Return the address of a concrete product associated with this concrete factory class.
		/// This method is virtual and should be overridden by every concrete factory that derives from the base class
		/// This is a polymorphic function call that is used by the family of factories to instantiate a new object of the correct type
		/// </summary>
		/// <returns>Pointer to the newly created object (user owns this memory)</returns>
		virtual gsl::owner <T*> Create() const = 0;
		
		/// <summary>
		/// Return a string representing the name of the class the factory instantiates
		/// </summary>
		/// <returns>String that is the name of the class that this factory can create</returns>
		virtual std::string ClassName() const = 0;


	protected:
		/// <summary>
		/// Given a reference to a concrete factory, add it to the list of factories for this abstract factory.
		/// Will throw an error if the user tries to register the same class Factory twice.
		/// </summary>
		/// <param name="factory">reference to the factory to be added</param>
		static void Add(const Factory<T>& factory);
		
		/// <summary>
		/// Given a reference to a concrete factory, remove it from the list of factories for this abstract factory.
		/// </summary>
		/// <param name="factory">reference to the factory to be removed</param>
		static void Remove(const Factory<T>& factory);

	private:

		inline static HashMap<const std::string, const Factory<T>* const> _factoryList;
	};

#define ConcreteFactory(ConcreteType, AbstractType)																				\
		class ConcreteType##Factory : public Factory<AbstractType>																\
		{																														\
		public:																													\
			ConcreteType##Factory(): _className(#ConcreteType) { Add(*this); }													\
			~ConcreteType##Factory() { Remove(*this); }																			\
			gsl::owner <AbstractType*> Create() const override { return new ConcreteType();  }									\
			std::string ClassName() const { return _className; }																\
		private:																												\
			std::string _className;																								\
		};																						

}

#include "Factory.inl"

