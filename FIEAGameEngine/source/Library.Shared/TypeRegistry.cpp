#include "pch.h"
#include "TypeRegistry.h"

namespace FIEAGameEngine
{

	TypeRegistry* TypeRegistry::_typeRegistryInstance = 0;

	const Vector<Signature>& TypeRegistry::GetSignatures(size_t typeID) 
	{
		return _registry.At(typeID);
	}

	TypeRegistry* TypeRegistry::GetInstance()
	{
		if (_typeRegistryInstance == nullptr)
		{
			_typeRegistryInstance = new TypeRegistry;
		}
		return _typeRegistryInstance;
	}
}