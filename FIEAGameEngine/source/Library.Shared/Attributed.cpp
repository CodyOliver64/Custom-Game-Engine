#include "pch.h"
#include "Attributed.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Attributed)


	Attributed::Attributed(size_t typeID)
	{
		Populate(typeID);
	}
	
	Attributed::Attributed(const Attributed& other)
		: Scope(other)
	{
		Repopulate(other.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& other)
		: Scope(std::move(other))
	{
		Repopulate(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		if (this != &other)
		{
			Scope::operator=(other);

			Repopulate(other.TypeIdInstance());
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other)
	{
		if (this != &other)
		{
			Scope::operator=(std::move(other));

			Repopulate(other.TypeIdInstance());
		}
		return *this;
	}
	
	void Attributed::Populate(size_t typeID)
	{
		(*this)["this"] = this;
		
		const Vector<Signature>& signatureList = TypeRegistry::GetInstance()->GetSignatures(typeID);
		_numPrescribedAttributes = signatureList.Size() + 1;

		for (auto signature : signatureList)
		{
			Datum& newDatum = Append(signature._name);

			if (newDatum.Type() != DatumTypes::Unknown && newDatum.Type() != signature._type)
			{
				throw std::runtime_error("Invalid - Key given exists with a different type");
			}

			newDatum.SetType(signature._type);
			
			
			if (signature._size > 0)
			{
				auto objectAddress = (reinterpret_cast<std::uint8_t*>(this) + signature._offset);
				newDatum.SetStorage(objectAddress, signature._size);
			}
		}
	}

	void Attributed::Repopulate(size_t typeID)
	{
		(*this)["this"] = this;

		const Vector<Signature>& signatureList = TypeRegistry::GetInstance()->GetSignatures(typeID);
		_numPrescribedAttributes = signatureList.Size() + 1;


		for (auto signature : signatureList)
		{
			if (signature._type != DatumTypes::Table)
			{
				auto objectAddress = (reinterpret_cast<std::uint8_t*>(this) + signature._offset);
				Find(signature._name)->SetStorage(objectAddress, signature._size);
			}
		}
	}
	
	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& key)
	{
		if (IsPrescribedAttribute(key))
		{
			throw std::runtime_error("Invalid - Key given is already Prescribed");
		}

		return Append(key);
	}
	
	bool Attributed::IsAttribute(const std::string& key)
	{
		return _map.ContainsKey(key);
	}
	bool Attributed::IsPrescribedAttribute(const std::string& key)
	{
		for (size_t i = 0; i < _numPrescribedAttributes; ++i)
		{
			if (_vector[i]->first == key)
			{
				return true;
			}
		}

		return false;
	}
	bool Attributed::IsAuxiliaryAttribute(const std::string& key)
	{
		for (size_t i = _numPrescribedAttributes; i < _vector.Size(); ++i)
		{
			if (_vector[i]->first == key)
			{
				return true;
			}
		}

		return false;
	}
}