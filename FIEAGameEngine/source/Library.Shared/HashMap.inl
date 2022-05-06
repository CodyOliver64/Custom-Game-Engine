#include "HashMap.h"

namespace FIEAGameEngine
{
	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(const size_t size, DefaultHash<TKey> hashFunctor, std::function<bool(const TKey&, const TKey&)> equalityFunctor) :
		_count(0),_hash(hashFunctor), _equals(equalityFunctor)
	{
		_map.Resize(size);
	}

	template <typename TKey, typename TData>
	std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const PairType& data)
	{
		size_t index = _hash(data.first) % _map.Size();

		typename SList<PairType>::Iterator it;

		if (_map[index].IsEmpty())
		{
			it = _map[index].PushBack(data);
		}
		else
		{

			for (it = _map[index].begin(); it != _map[index].end(); ++it)
			{
				if ((_equals((*it).first, data.first)))
				{
					return std::make_pair(Iterator(*this, it, index), false);
				}
			}

			it = _map[index].PushBack(data);
		}

		_count++;

		return std::make_pair(Iterator(*this, it, index), true);
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key)
	{
		size_t index = _hash(key) % _map.Size();
		typename SList<PairType>::Iterator it;

		for (it = _map[index].begin(); it != _map[index].end(); ++it)
		{
			if (_equals((*it).first, key))
			{
				break;
			}
		}

		if (it == _map[index].end())
		{
			return end();
		}

		return Iterator(*this, it, index);
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& key) const
	{
		size_t index = _hash(key) % _map.Size();
		typename SList<PairType>::ConstIterator it;

		for (it = _map[index].begin(); it != _map[index].end(); ++it)
		{
			if (_equals((*it).first, key))
			{
				break;
			}
		}

		if (it == _map[index].end())
		{
			return end();
		}

		return ConstIterator(*this, it, index);
	}

	template <typename TKey, typename TData>
	void HashMap<TKey, TData>::Remove(const TKey& key)
	{
		Iterator it = Find(key);

		if (it != end())
		{
			_map[it._index].Remove(it._chainIT);
			--_count;
		}
	}

	template <typename TKey, typename TData>
	void HashMap<TKey, TData>::Clear()
	{
		for (size_t i = 0; i < _map.Size(); ++i)
		{
			_map[i].Clear();
		}
		_count = 0;
	}

	template <typename TKey, typename TData>
	inline const std::size_t HashMap<TKey, TData>::Size() const
	{
		return _count;
	}

	template <typename TKey, typename TData>
	const float HashMap<TKey, TData>::LoadFactor() const
	{
		int filledBuckets = 0;
		for (size_t i = 0; i < _map.Size(); ++i)
		{
			if (!(_map[i].IsEmpty()))
			{
				filledBuckets++;
			}
		}
		
		return (static_cast<float>(filledBuckets)) / _map.Size() * 100;
	}

	template <typename TKey, typename TData>
	inline const bool HashMap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		return (Find(key) != end());
	}

	template <typename TKey, typename TData>
	TData& HashMap<TKey, TData>::At(const TKey& key)
	{
		Iterator it = Find(key);

		if (it == end())
		{
			throw std::runtime_error("Invalid - Key Not In List");
		}

		return (*it).second;
	}

	template <typename TKey, typename TData>
	const TData& HashMap<TKey, TData>::At(const TKey& key) const
	{
		ConstIterator it = Find(key);

		if (it == end())
		{
			throw std::runtime_error("Invalid - Key Not In List");
		}

		return (*it).second;
	}
	
	template <typename TKey, typename TData>
	TData& HashMap<TKey, TData>::operator[](const TKey& key)
	{
		Iterator it = Find(key);

		if (it == end())
		{
			it = Insert(std::make_pair(key, TData())).first;  //FIX ME MAYBE - this Insert loops to check if we need to insert unnecessarily 
		}

		return (*it).second;
		
	}

	template <typename TKey, typename TData>
	const TData& HashMap<TKey, TData>::operator[](const TKey& key) const
	{
		return At(key);
	}

	template <typename TKey, typename TData>
	bool HashMap<TKey, TData>::Rehash(const size_t size)
	{
		if (size < 2)
		{
			return false;
		}
		
		BucketType newMap(size);
		newMap.Resize(size);

		size_t newIndex;
		for (Iterator it = begin(); it != end(); ++it)
		{
			newIndex = _hash((*it).first) % size;
			newMap[newIndex].PushBack((*it));
		}

		_map = std::move(newMap);
		return true;
	}


	//--------------------------------------------------------------------Iterator Class-----------------------------------------------------------------------------------------------


	//Constructors
	template <typename TKey, typename TData>
	HashMap<TKey, TData>::Iterator::Iterator(HashMap& owner, const typename SList<PairType>::Iterator& it, std::size_t index) :
		_owner(&owner), _chainIT(it), _index(index)
	{
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::ConstIterator::ConstIterator (const HashMap& owner, const typename SList<PairType>::ConstIterator& it, std::size_t index) :
		_owner(&owner), _chainIT(it), _index(index)
	{
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _chainIT(other._chainIT), _index(other._index)
	{
	}


	//Iterator Member Functions
	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not assigned to a data structure");
		}

		_chainIT++;

		if (_chainIT == _owner->_map[_index].end())
		{
			if (_index < _owner->_map.Size())
			{

				_index++;

				if (_index != _owner->_map.Size())
				{
					while (_index != _owner->_map.Size() && _owner->_map[_index].IsEmpty())
					{
						_index++;
					}

					if (_index < _owner->_map.Size())
					{
						_chainIT = _owner->_map[_index].begin();
					}
				}
			}
		}

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not assigned to a data structure");
		}

		_chainIT++;

		if (_chainIT == _owner->_map[_index].end())
		{
			if (_index < _owner->_map.Size())
			{

				_index++;

				if (_index != _owner->_map.Size())
				{
					while (_owner->_map[_index].IsEmpty() && _index != _owner->_map.Size()) //dont need - 1
					{
						_index++;
					}

					if (_index < _owner->_map.Size())
					{
						_chainIT = _owner->_map[_index].begin();
					}
				}
			}
		}

		return *this;
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();
		return temp;
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator!=(const Iterator& other) const
	{
		return (_owner != other._owner || _index != other._index || _chainIT != other._chainIT);
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (_owner != other._owner || _index != other._index || _chainIT != other._chainIT);
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator==(const Iterator& other) const
	{
		return (_owner == other._owner && _index == other._index && _chainIT == other._chainIT);
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return (_owner == other._owner && _index == other._index && _chainIT == other._chainIT);
	}

	template <typename TKey, typename TData>
	inline std::pair<const TKey, TData>& HashMap<TKey, TData>::Iterator::operator*() const
	{
		if (_index >= _owner->_map.Size())
		{
			throw std::runtime_error("Index of this iterator is out of bounds");
		}

		return *(_chainIT);
	}

	template <typename TKey, typename TData>
	inline const std::pair<const TKey, TData>& HashMap<TKey, TData>::ConstIterator::operator*() const
	{
		if (_index >= _owner->_map.Size())
		{
			throw std::runtime_error("Index of this iterator is out of bounds");
		}

		return *(_chainIT);
	}

	template <typename TKey, typename TData>
	inline std::pair<const TKey, TData>* HashMap<TKey, TData>::Iterator::operator->() const
	{
		return  &operator*();
	}

	template <typename TKey, typename TData>
	inline const std::pair<const TKey, TData>* HashMap<TKey, TData>::ConstIterator::operator->() const
	{
		return  &operator*();
	}

	//HashMap Member Functions
	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::begin()
	{
		size_t index;

		if (_count != 0)
		{
			for (index = 0; index < _map.Size(); ++index)
			{
				if (!(_map[index].IsEmpty()))
				{
					return Iterator(*this, _map[index].begin(), index);
				}
			}
		}

		return end();
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::begin() const
	{
		size_t index;

		if (_count != 0)
		{
			for (index = 0; index < _map.Size(); ++index)
			{
				if (!(_map[index].IsEmpty()))
				{
					return ConstIterator(*this, _map[index].cbegin(), index);
				}
			}
		}

		return cend();
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cbegin() const
	{
		size_t index;

		if (_count != 0)
		{
			for (index = 0; index < _map.Size(); ++index)
			{
				if (!(_map[index].IsEmpty()))
				{
					return ConstIterator(*this, _map[index].cbegin(), index);
				}
			}
		}

		return cend();
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::end()
	{
		return Iterator(*this, _map[_map.Size() - 1].end(), _map.Size());
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::end() const
	{
		return ConstIterator(*this, _map[_map.Size() - 1].cend(), _map.Size());
	}

	template <typename TKey, typename TData>
	inline typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cend() const
	{
		return ConstIterator(*this, _map[_map.Size() - 1].cend(), _map.Size());
	}

}