#include "Vector.h"

namespace FIEAGameEngine
{
	template <typename T>
	Vector<T>::Vector(const size_t capacity) :
		_size(0), _capacity(capacity)
	{
		_vArray = reinterpret_cast<T*>(malloc(sizeof(T) * _capacity));
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		Clear();
		free(_vArray);
	}

	template <typename T>
	Vector<T>::Vector(std::initializer_list<T> list)
	{
		Reserve(list.size());
		
		for (const auto& item : list)
		{
			PushBack(item);
		}
	}	
	
	template <typename T>
	Vector<T>::Vector(const Vector& other)
	{
		_vArray = reinterpret_cast<T*>(malloc(sizeof(T) * other._capacity));
		_capacity = other._capacity;

		for (size_t i = 0; i < other._size; ++i)
		{
			PushBack(*(other._vArray + i));
		}
	}

	template <typename T>
	Vector<T>::Vector(Vector&& other) noexcept :
		_size(other._size), _capacity(other._capacity), _vArray(other._vArray)
	{
		other._size = 0;
		other._capacity = 0;
		other._vArray = nullptr;
	}

	


	template <typename T>
	T& Vector<T>::Front()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_vArray);
	}
	template <typename T>
	const T& Vector<T>::Front() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_vArray);
	}

	template <typename T>
	T& Vector<T>::Back()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_vArray + (_size - 1));
	}
	template <typename T>
	const T& Vector<T>::Back() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_vArray + (_size - 1));
	}
	
	template <typename T>
	inline void Vector<T>::PopBack()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}
		
		(_vArray + (--_size))->~T();
	}

	template<typename T>
	void Vector<T>::Remove(Iterator it)
	{
		if (_size > 0 && it._index < _size)
		{
			(_vArray + (it._index))->~T();

			std::memmove((_vArray + it._index), (_vArray + it._index + 1), (sizeof(T) * (_size - it._index - 1)));

			--_size;
		}
	}

	template<typename T>
	inline void Vector<T>::Remove(const T& value, std::function<bool(const T&, const T&)> equalityFunctor)
	{
		Remove(Find(value, equalityFunctor));
	}

	template<typename T>
	void Vector<T>::Remove(const Iterator& it1, const Iterator& it2)
	{
		if (it1._owner != this || it2._owner != this)
		{
			return;
		}

		else if (it1._index > it2._index)
		{
			return;
		}
		size_t loopTimes = it2._index - it1._index;


		for (size_t i = it1._index; i < it2._index; ++i)
		{
			(_vArray + i)-> ~T();
		}



		std::memmove(_vArray + it1._index, (_vArray + (it2._index)), sizeof(T) * (_size - it2._index));
		_size = _size - loopTimes;
	}


	template <typename T>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T& data)
	{
		new(_vArray + _size++)T(data);
		
		if (_size == _capacity)
		{
			Reserve(_capacity * 2);
		}

		return Iterator(*this, (_size - 1));
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		for (size_t i = 0; i < _size; ++i)
		{
			(_vArray + i)->~T();
		}

		_size = 0;
	}

	template <typename T>
	void Vector<T>::Reserve(const std::size_t newCapacity)
	{
		if (newCapacity > _capacity)
		{
			void* ptr = std::realloc(_vArray, sizeof(T) * newCapacity);
			if (ptr == nullptr)
			{
				throw std::runtime_error("Failed to allocate memory...");
			}
			//assert(ptr != nullptr);
			_vArray = reinterpret_cast<T*>(ptr);
			_capacity = newCapacity;
		}
	}

	template <typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return (_size == 0);
	}

	template <typename T>
	const std::size_t Vector<T>::Size() const
	{
		return _size;
	}

	template <typename T>
	const std::size_t Vector<T>::Capacity() const
	{
		return _capacity;
	}

	template <typename T>
	inline T& Vector<T>::At(const std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}

		return *(_vArray + index);
	}

	template <typename T>
	inline const T& Vector<T>::At(const std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}

		return *(_vArray + index);
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor)
	{
		Iterator it;

		for (it = begin(); it != end(); ++it)
		{
			if (equalityFunctor(*it, value))
			{
				break;
			}
		}

		return it;
	}

	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor) const
	{
		ConstIterator it;

		for (it = begin(); it != end(); ++it)
		{
			if (equalityFunctor(*it, value))
			{
				break;
			}
		}

		return it;
	}

	
	template <typename T>
	inline T& Vector<T>::operator[](const std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}
		
		return *(_vArray + index);
	}
	template <typename T>
	inline const T& Vector<T>::operator[](const std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}
		
		return *(_vArray + index);
	}
	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Clear();

			for (size_t i = 0; i < other._size; ++i)
			{
				PushBack(*(other._vArray + i));
			}
		}

		return *this;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			free(_vArray);

			_size = other._size;
			_capacity = other._capacity;
			_vArray = other._vArray;

			other._size = 0;
			other._capacity = 0;
			other._vArray = nullptr;
		}

		return *this;
	}

	template <typename T>
	void Vector<T>::Resize(std::size_t size)
	{
		if (size < _size)
		{
			for (size_t i = size; i < _size; ++i)
			{
				_vArray[i].~T();
				_size--;
			}
		}
		else
		{
			Reserve(size);

			for (size_t i = _size; i < size; ++i)
			{
				new (_vArray + i)T();
				_size++;
			}
		}
	}

	template <typename T>
	Vector<T> Vector<T>::operator+(const Vector& other)
	{
		Reserve(other.Size() + _size);

		Vector<T> newVector = *this;

		for (size_t i = 0; i < other._size; ++i)
		{
			newVector.PushBack(*(other._vArray + i));
		}
		return newVector;
	}


	//--------------------------------------------------------------------Iterator Class-----------------------------------------------------------------------------------------------


	//Constructors
	template<typename T>
	Vector<T>::Iterator::Iterator(const Vector& owner, std::size_t index) :
		_owner(&owner), _index(index)
	{

	}

	template<typename T>
	Vector<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _index(other._index)
	{
	}


	//Vector Member Functions
	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(Iterator(*this, 0));
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(Iterator(*this, 0));
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(*this, _size);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(Iterator(*this, _size));
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(Iterator(*this, _size));
	}


	//Iterator Member Functions
	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not assigned to a data structure");
		}

		if (_index != _owner->_size)
		{
			_index++;
		}

		return *this;
	}

	template<typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not assigned to a data structure");
		}

		if (_index != _owner->_size)
		{
			_index++;
		}

		return *this;
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not assigned to a data structure");
		}

		if (_index > 0)
		{
			--_index;
		}

		return *this;
	}

	template<typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not assigned to a data structure");
		}

		if (_index > 0)
		{
			--_index;
		}

		return *this;
	}



	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator temp = *this;
		operator--();
		return temp;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator temp = *this;
		operator--();
		return temp;
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& other) const
	{
		return (_owner != other._owner || _index != other._index);
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (_owner != other._owner || _index != other._index);
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& other) const
	{
		return (_owner == other._owner && _index == other._index);
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return (_owner == other._owner && _index == other._index);
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (_index >= _owner->_size)
		{
			throw std::runtime_error("Index of this iterator is out of bounds");
		}

		return *(_owner->_vArray + _index);
	}

	template<typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		if (_index >= _owner->_size)
		{
			throw std::runtime_error("Index of this iterator is out of bounds");
		}

		return *(_owner->_vArray + _index);
	}
}