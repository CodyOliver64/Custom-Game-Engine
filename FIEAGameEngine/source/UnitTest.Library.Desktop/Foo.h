#pragma once

#include <cstdint>
#include "RTTI.h"
#include "Factory.h"
namespace FIEAGameEngine
{
	/// <summary>
	/// Test class used to test deep copy memory allocation with our SList data structure
	/// </summary>
	class Foo : public RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI)

	public:
		
		/// <summary>
		/// Constructor for a Foo that initalizes the Foo with an number
		/// </summary>
		/// <param name="data"> Takes in an int32_t that is used to set the data of the foo</param>
		Foo(std::int32_t data = 999);
		
		/// <summary>
		/// Copy Constructor that sets a new Foo with its own unique pointer that points to equal data
		/// </summary>
		/// <param name="other">Takes in a constant foo reference used to set the new foo copy</param>
		Foo(const Foo& other);
		
		/// <summary>
		/// Equal to Operator used to compare Foo's. Foo's are equal if they point to ints that are equivalent in value
		/// </summary>
		/// <param name="other">Takes in a Foo that will be compared against</param>
		/// <returns>Returns true if they are equal and false if they are not</returns>
		Foo& operator=(const Foo& other);

		//Foo Deconstructor, frees the data pointer on deconstruction
		virtual ~Foo();

		/// <summary>
		/// Returns a reference to the data pointed to by Data
		/// </summary>
		/// <returns>Returns an int32_t reference</returns>
		int32_t& Data();
		
		/// <summary>
		/// Const Version of data that can be called by a const Foo
		/// </summary>
		/// <returns>Returns an int32_t reference</returns>
		int32_t& Data() const;
		
		/// <summary>
		/// Equal To Operator, returns true if the two Foo's have Data pointers that point to equivalent ints. Will also return true if they are the same identical foo
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns>Returns true or false depending if the foos are equal</returns>
		bool operator==(const Foo& rhs) const;
		
		/// <summary>
		/// Not Equal To Operator, returns false if the two Foo's have Data pointers that point to equivalent ints. Will also return false if they are the same identical foo. Calls Equal To Operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns>Returns true or false depending if the foos are equal</returns>
		bool operator!=(const Foo& rhs) const;

	private:
		std::int32_t* mData;
	};

	/*class FooFactory : public Factory<RTTI>
	{																													
	public:
		FooFactory(): _className("Foo") { Add(*this); }
		~FooFactory() { Remove(*this); }																	
		gsl::owner <RTTI*> Create() const { return new Foo(); }										
		std::string ClassName() const { return _className; }
	private:
		std::string _className;
	};*/

	ConcreteFactory(Foo, RTTI)
}
