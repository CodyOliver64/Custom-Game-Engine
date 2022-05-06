#include "pch.h"
#include "Scope.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Scope)

		Scope::Scope(std::size_t capacity) :
		_vector(capacity), _map(capacity)
	{
	}

	Scope::Scope(const Scope& other) :
		_map(other._map.Size()), _vector(other._vector.Capacity())
	{
		for (HashMap<std::string, Datum>::PairType* entry : other._vector)
		{
			if ((entry->second.Type() == DatumTypes::Table && !entry->second.IsExternal()) || entry->second.Type() != DatumTypes::Table)
			{
				Datum newDatum;
				if (entry->second.Type() == DatumTypes::Table)
				{
					newDatum.SetType(DatumTypes::Table);
					newDatum.Reserve(entry->second.Size());
					for (std::size_t i = 0; i < entry->second.Size(); ++i)
					{
						Scope& nestedScope = *(entry->second.GetScope(i));
						Scope* childCopy = nestedScope.Clone();
						childCopy->_parent = this;
						newDatum.PushBack(childCopy);
					}
				}
				else
				{
					newDatum = entry->second;
				}

				auto pair = _map.Insert(std::make_pair(entry->first, newDatum));
				_vector.PushBack(&(*(pair.first)));
			}
		}
	}

	Scope::Scope(Scope&& other) noexcept :
		_map{ std::move(other._map) }, _vector{ std::move(other._vector) }
	{
		//Link parent of old to the new object by updating the scope pointer
		if (other._parent != nullptr)
		{
			_parent = other._parent;
			auto result = _parent->FindContainedScope(&other);
			result.first->Set(this, result.second);
		}

		//Link children of old to point to the new object
		Vector<typename HashMap<std::string, Datum>::PairType*>::Iterator it;
		for (it = _vector.begin(); it != _vector.end(); ++it)
		{
			if ((*it)->second.Type() == DatumTypes::Table)
			{
				for (size_t i = 0; i < (*it)->second.Size(); ++i)
				{
					(*it)->second[i]._parent = this;
				}
			}
		}
		
		other._parent = nullptr;
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (this != &other)
		{
			Orphan();
			Clear();
			_map.Clear();
			_vector.Clear();

			for (HashMap<std::string, Datum>::PairType* entry : other._vector)
			{
				Datum newDatum;
				if (entry->second.Type() == DatumTypes::Table)
				{
					newDatum.SetType(DatumTypes::Table);
					newDatum.Reserve(entry->second.Size());
					for (std::size_t i = 0; i < entry->second.Size(); ++i)
					{
						Scope& nestedScope = *(entry->second.GetScope(i));
						Scope* childCopy = nestedScope.Clone();
						childCopy->_parent = this;
						newDatum.PushBack(childCopy);
					}
				}
				else
				{
					newDatum = entry->second;
				}

				auto pair = _map.Insert(std::make_pair(entry->first, newDatum));
				_vector.PushBack(&(*(pair.first)));
			}


		}
		return *this;
	}

	Scope& Scope::operator=(Scope&& other)
	{
		if (this != &other)
		{
			//Clear destination of move
			Clear();
			_map.Clear();
			_vector.Clear();

			//move private members
			_map = std::move(other._map);
			_vector = std::move(other._vector);

			//Link parent of old to the new object by updating the scope pointer
			if (other._parent != nullptr)
			{
				_parent = other._parent;
				auto result = _parent->FindContainedScope(&other);
				result.first->Set(this, result.second);
			}

			//Link children of old to point to the new object
			Vector<typename HashMap<std::string, Datum>::PairType*>::Iterator it;
			for (it = _vector.begin(); it != _vector.end(); ++it)
			{
				if ((*it)->second.Type() == DatumTypes::Table)
				{
					for (size_t i = 0; i < (*it)->second.Size(); ++i)
					{
						(*it)->second[i]._parent = this;
					}
				}
			}

			other._parent = nullptr;
		}

		return *this;
	}

	Scope::~Scope()
	{
		Orphan();
		Clear();
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}


	Datum* Scope::Find(const std::string& key)
	{
		HashMap<std::string, Datum>::Iterator it;
		it = _map.Find(key);
		if (it != _map.end())
		{
			return &(*it).second;
		}

		return nullptr;
	}

	const Datum* Scope::Find(const std::string& key) const
	{
		HashMap<std::string, Datum>::ConstIterator it;
		it = _map.Find(key);
		if (it != _map.end())
		{
			return &(*it).second;
		}

		return nullptr;
	}

	Datum* Scope::Search(const std::string& key, Scope*& scopeResult)
	{
		Datum* datumResult = Find(key);

		if (datumResult != nullptr)
		{
			scopeResult = this;
		}
		else if (_parent != nullptr)
		{
			datumResult = _parent->Search(key, scopeResult);
		}
		
		return datumResult;
	}

	const Datum* Scope::Search(const std::string& key, const Scope*& scopeResult) const
	{
		const Datum* datumResult = Find(key);

		if (datumResult != nullptr)
		{
			scopeResult = this;
		}
		else if (_parent != nullptr)
		{
			datumResult = _parent->Search(key, scopeResult);
		}

		return datumResult;
	}

	Datum* Scope::Search(const std::string& key)
	{
		Datum* datumResult = Find(key);

		if (_parent != nullptr && datumResult == nullptr)
		{
			datumResult = _parent->Search(key);
		}

		return datumResult;
	}

	const Datum* Scope::Search(const std::string& key) const
	{
		const Datum* datumResult = Find(key);

		if (_parent != nullptr && datumResult == nullptr)
		{
			datumResult = _parent->Search(key);
		}

		return datumResult;
	}

	Datum& Scope::Append(const std::string& key)
	{
		auto it = _map.Insert(std::make_pair(key, Datum()));  //Why does move get called twice here and is that ok?

		if (it.second == true)
		{
			_vector.PushBack(&(*it.first));
		}

		return it.first->second;
	}

	Scope& Scope::AppendScope(const std::string& key)
	{
		Datum* scopeResult = Find(key);

		if (scopeResult == nullptr)
		{
			Scope* newScope = new Scope();
			newScope->_parent = this;
			auto it = _map.Insert(std::make_pair(key, Datum(newScope)));
			_vector.PushBack(&(*it.first));
			return *newScope;
		}

		if (scopeResult->Type() != DatumTypes::Table && scopeResult->Type() != DatumTypes::Unknown)
		{
			throw std::runtime_error("Invalid - Key is present and is not of type Table");
		}

		Scope* newScope = new Scope();
		newScope->_parent = this;
		scopeResult->PushBack(newScope);
		return *newScope;
	}

	void Scope::Adopt(Scope& child, const std::string& key) //check if self cuz cant adopt your self
	{
		if (this == &child || IsDescendantOf(child))
		{
			throw std::runtime_error("Invalid Operation, Can not adopt the given scope as a child");
		}
		
		Datum* scopeResult = Find(key);

		if (scopeResult == nullptr)
		{
			auto it = _map.Insert(std::make_pair(key, Datum(&child)));
			_vector.PushBack(&(*it.first));
		}

		else
		{
			if (scopeResult->Type() != DatumTypes::Table && scopeResult->Type() != DatumTypes::Unknown)
			{
				throw std::runtime_error("Invalid - Key is present and is not of type Table");
			}
			scopeResult->PushBack(&child);
		}

		if (child._parent != nullptr)
		{
			child.Orphan();
		}

		child._parent = this;
	}

	std::pair<Datum*, std::size_t> Scope::FindContainedScope(Scope* const searchingFor) //might need to be a reference, if not should check if nullptr
	{
		Vector<typename HashMap<std::string, Datum>::PairType*>::Iterator it;
		for (it = _vector.begin(); it != _vector.end(); ++it)
		{
			if ((*it)->second.Type() == DatumTypes::Table)
			{
				auto result = (*it)->second.Find(searchingFor);
				if (result.second == true)
				{
					return std::make_pair(&(*it)->second, result.first);
				}
			}
		}

		return std::make_pair(nullptr, 0);
	}

	void Scope::Orphan()
	{
		if (_parent != nullptr && _isExternalScope == false)
		{
			auto result = _parent->FindContainedScope(this);
			assert(result.first != nullptr);
			result.first->RemoveAt(result.second);
			_parent = nullptr;
		}
	}

	void Scope::Clear()
	{
		if (_isExternalScope)return;
		Vector<typename HashMap<std::string, Datum>::PairType*>::Iterator it;
		for (it = _vector.begin(); it != _vector.end(); ++it)
		{
			//auto& [key, datum] = **it;
			if ((*it)->second.Type() == DatumTypes::Table && !(*it)->second.IsExternal())
			{
				for (size_t i = 0; i < (*it)->second.Size(); ++i)
				{
					(*it)->second[i]._parent = nullptr; //no need to oprhan children if its parent is being deleted

					if ((*it)->second[i]._isExternalScope == false)
					{
						//(*it)->second[i]._parent = nullptr; //no need to oprhan children if its parent is being deleted
						delete& ((*it)->second[i]);
					}
				}
			}
		}
	}

	Scope* Scope::GetParent() const
	{
		return _parent;
	}

	Datum& Scope::operator[](const std::string& key)
	{
		return Append(key);
	}

	Datum& Scope::operator[](size_t index)
	{
		return _vector[index]->second;
	}

	bool Scope::operator==(const Scope& other) const
	{
		if (this == &other) return true;

		if (&other == nullptr || _vector.Size() != other._vector.Size())
		{
			return false;
		}

		for (size_t i = 0; i < _vector.Size(); ++i)
		{
			const Datum* otherDatum = other.Find(_vector[i]->first);

			if (otherDatum == nullptr || otherDatum->Type() != _vector[i]->second.Type()) //might not need type check here since datum will do it
			{
				return false;
			}
			if (_vector[i]->second != *otherDatum)
			{
				return false;
			}
		}

		return true;
	}

	bool Scope::operator!=(const Scope& other) const
	{
		return !(operator==(other));
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (rhs != nullptr && rhs->Is(Scope::TypeIdClass()))
		{
			const Scope* scope = reinterpret_cast<const Scope*>(rhs);
			return(operator==(*scope));
		}

		return false;
	}

	std::size_t Scope::vectorSize() const
	{
		return _vector.Size();
	}
	std::size_t Scope::hashMapSize() const
	{
		return _map.Size();
	}


	bool Scope::IsAncestorOf(const Scope& scope) const
	{
		Scope* parent{ scope._parent };
		while (parent != nullptr)
		{
			if (parent == this) return true;
			parent = parent->_parent;
		}
		return false;
	}

	bool Scope::IsDescendantOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}

	Datum& Scope::At(const std::string key)
	{
		Datum* datum = Find(key);
		if (datum == nullptr)
		{
			throw std::runtime_error("Expected entry.");
		}

		return *datum;
	}

	const Datum& Scope::At(const std::string key) const
	{
		const Datum* datum = Find(key);
		if (datum == nullptr)
		{
			throw std::runtime_error("Expected entry.");
		}

		return *datum;
	}
}