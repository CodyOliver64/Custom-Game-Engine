#include "SList.h"


namespace FIEAGameEngine
{
	template <typename T>
	SList<T>::SList() :
		_size(0), _front(nullptr), _back(nullptr)
	{

	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	
	template <typename T>
	SList<T>::SList(const SList& other)
	{
		Node* temp = other._front;

		while (temp != nullptr)
		{
			PushBack(temp->Data);
			temp = temp->Next;
		}
	}

	template <typename T>
	SList<T>::Node::Node(const T& data, Node* next) :
		Data(data), Next(next)
	{
	}


	template <typename T>
	T& SList<T>::Front()
	{
		if (_front == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _front->Data;
	}
	template <typename T>
	const T& SList<T>::Front() const
	{
		if (_front == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _front->Data;
	}


	template <typename T>
	T& SList<T>::Back()
	{
		if (_back == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _back->Data;
	}
	template <typename T>
	const T& SList<T>::Back() const
	{
		if (_back == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _back->Data;
	}


	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& data)
	{
		Node* newNode = new Node(data, _front);

		_front = newNode;
		_size++;

		if (_size == 1)
		{
			_back = newNode;
		}

		return Iterator(*this, newNode);
	}


	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& data)
	{
		if (_size == 0)
		{
			return PushFront(data);
		}

		Node* newNode = new Node(data, nullptr);

		_back->Next = newNode;
		_back = newNode;
		_size++;

		return Iterator(*this, newNode);
	}


	template <typename T>
	void SList<T>::PopFront()
	{
		if (_front == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		Node* temp = _front;
		_front = temp->Next;
		delete temp;
		_size--;

		if (_size == 0)
		{
			_back = nullptr;
		}
	}


	template <typename T>
	void SList<T>::PopBack()
	{
		if (_back == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}


		if (_size == 1)
		{
			delete _back;
			_back = nullptr;
			_front = nullptr;
			_size--;
			return;
		}

		Node* newBack = _front;
		for (int i = 0; i < (int)(_size - 2); ++i)
		{
			newBack = newBack->Next;
		}

		delete _back;
		_back = newBack;
		_back->Next = nullptr;
		_size--;
	}


	template <typename T>
	void SList<T>::Clear()
	{
		Node* temp = _front;
		while (temp != nullptr)
		{
			Node* next = temp->Next;
			delete temp;
			temp = next;
		}

		_front = _back = nullptr;
		_size = 0;
	}


	template <typename T>
	bool SList<T>::IsEmpty() const
	{
		if (_size == 0)
		{
			return true;
		}

		return false;

		//return (_size == 0); inline this cuz its smole
	}


	template <typename T>
	const std::size_t SList<T>::Size() const
	{
		return _size;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			this->Clear();

			Node* temp = other._front;

			while (temp != nullptr)
			{
				PushBack(temp->Data);
				temp = temp->Next;
			}
		}

		return *this;
	}


	template <typename T>
	void SList<T>::Remove(Iterator it)
	{
		if (it._owner != this)
		{
			throw std::runtime_error("Iterator passed was not assigned to this list");
		}

		if (it._node == _front)
		{
			PopFront();
		}
		else if (it._node == _back)
		{
			PopBack();
		}
		else
		{
			Node* temp = begin()._node;
			while (temp != nullptr && temp->Next != nullptr)
			{
				if (temp->Next == it._node)
				{
					Node* deleteNode = temp->Next;
					temp->Next = deleteNode->Next;
					delete deleteNode;
					--_size;
					break;
				}
			}
		}
	}

	//--------------------------------------------------------------------Iterator Class-----------------------------------------------------------------------------------------------

	
	//Constructors
	template<typename T>
	SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		_owner(&owner), _node(node)
	{

	}
	
	template<typename T>
	SList<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _node(other._node)
	{
	}
	
	
	//SList Member Functions
	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator(Iterator(*this, _front));
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(Iterator(*this, _front));
	}
	
	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator(Iterator(*this));
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(Iterator(*this));
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor)
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
	typename SList<T>::ConstIterator SList<T>::Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor) const
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

	template<typename T>
	void SList<T>::Remove(const T& value, std::function<bool(const T&, const T&)> equalityFunctor)
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot remove from an empty list.");
		}
		
		Iterator current = begin();

		//Handle the case if we're removing the head
		if (equalityFunctor(*current, value))
		{
			PopFront();
		}

		else
		{
			Iterator previous = current;
			++current;
			for (current; current != end(); ++current)
			{
				if (equalityFunctor(*current, value))
				{
					//Handle the case if we're removing the tail
					if (current._node == _back)
					{
						PopBack();
					}
					
					else
					{
						previous._node->Next = current._node->Next;
						delete current._node;
						_size--;
					}
					return;
				}

				previous = current;
				++current;
			}
		}
	}


	template<typename T>
	void SList<T>::InsertAfter(const T& value, Iterator previous)
	{
		if (previous._owner != this)
		{
			throw std::runtime_error("Iterator passed was not assigned to this list");
		}
		
		if (previous._node == nullptr || previous._node == _back)
		{
			PushBack(value);
		}

		else
		{
			previous._node->Next = new Node(value, previous._node->Next);
			_size++;
		}
	}
	
	//Iterator Member Functions
	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not assigned to a data structure");
		}
		
		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template<typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not assigned to a data structure");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return (_node != other._node);
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (_node != other._node);
	}
	
	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return (_node == other._node);
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return (_node == other._node);
	}

	template<typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Can not dereference an iterator with a null node");
		}
		
		return _node->Data;
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Can not dereference an iterator with a null node");
		}
		
		return _node->Data;
	}
}