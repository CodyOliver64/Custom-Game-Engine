#pragma once
#include "Datum.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Signature is a thin struct that is used to hold all of the information required to dynamically build a scope based on its matching class
	/// These structs are build on registration by making a call to the Signature() method that needs to be supported by an Attributed object
	/// </summary>
	struct Signature final
	{
		
	public:
		std::string _name;
		DatumTypes _type;
		std::size_t _size;
		std::size_t _offset;
	};
}
