#pragma once
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <functional>

namespace FIEAGameEngine
{
	template <typename T>
	/// <summary>
	/// SList is a Singly Linked List data structure that has been template to accept any primitive or user defined types
	/// </summary>
	/// <typeparam name="T">The specified type(T) will dictated what the list will hold</typeparam>
	class SList final
	{

	private:
		
		struct Node final
		{
			T Data;
			Node* Next;

			Node(const T& data, Node* Next);
		};

	public:

		class Iterator;
		class ConstIterator;

		/// <summary>
		/// SList Constructor. Sets size to zero and front/back to nullptr
		/// </summary>
		SList();
		
		//SList Destructor that deletes all nodes that are currently in the list
		~SList();
		
		/// <summary>
		/// Copy Constructor for SList. Creates a new SList with its own nodes in memory that store equivalent data as the SList being copied from
		/// </summary>
		/// <param name="other">Takes in a const Slist to be copied from</param>
		SList(const SList& other);

		/// <summary>
		/// Will return a reference to the data held in the front node of the list
		/// </summary>
		/// <returns>T Reference</returns>
		T& Front();
		
		/// <summary>
		/// Const version of front that can be called by a const list to return a const reference to data
		/// </summary>
		/// <returns>const T Reference</returns>
		const T& Front() const; 

		/// <summary>
		/// Will return a reference to the data held in the back node of the list
		/// </summary>
		/// <returns>T Reference</returns>
		T& Back();
		
		/// <summary>
		/// Const version of back that can be called by a const list to return a const reference to data
		/// </summary>
		/// <returns>const T Reference</returns>
		const T& Back() const;

		/// <summary>
		/// Adds a node to the front of the list that contains the data of Type T passed into the function
		/// </summary>
		/// <param name="data">Data to be held by the new Node</param>
		SList<T>::Iterator PushFront(const T& data); //FIX ME TO RETURN ITERATOR

		/// <summary>
		/// Adds a node to the front of the list that contains the data of Type T passed into the function
		/// </summary>
		/// <param name="data">Data to be held by the new Node</param>
		SList<T>::Iterator PushBack(const T& data); //FIX ME TO RETURN ITERATOR

		/// <summary>
		/// Removes and deletes the front Node in the list
		/// </summary>
		void PopFront();

		/// <summary>
		/// Removes and deletes the back Node in the list
		/// </summary>
		void PopBack();

		/// <summary>
		/// Removes all Nodes from the list, leaving an empty list with size 0
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns true if the list is empty, false if not
		/// </summary>
		/// <returns></returns>
		bool IsEmpty() const;

		/// <summary>
		/// Returns the current size of the list (how many nodes it contains)
		/// </summary>
		/// <returns></returns>
		const std::size_t Size() const;

		void Remove(Iterator it);

		/// <summary>
		/// Assignment Operator - Sets one list equal to another by removing nodes in the list being changed and adding new unique nodes that contain equivalent data to the other list
		/// Results in a list of equal size
		/// </summary>
		/// <param name="other">Const reference to the other list to be copied from</param>
		/// <returns></returns>
		SList& operator=(const SList& other);

	private:

		std::size_t _size{ 0 };
		Node* _front{ nullptr };
		Node* _back{ nullptr };

	//--------------------------------------------------------------------Iterator Class-----------------------------------------------------------------------------------------------
	public:
		
		/// <summary>
		/// A class to  iterate and mutate the private members of an SList
		/// </summary>
		class Iterator
		{
			friend SList;

		public:
			Iterator() = default;
			Iterator(const Iterator & other) = default;
			Iterator& operator=(const Iterator & other) = default;
			~Iterator() = default;
			
			/// <summary>
			/// Equal operator that returns true if two iterators point to the same node
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns></returns>
			bool operator==(const Iterator& other) const;
			
			
			/// <summary>
			/// Not Equal operator that returns true if two iterators point to different nodes
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns></returns>
			bool operator!=(const Iterator& other) const;

			
			/// <summary>
			/// Reassigns the Iterator to the next node in the list (PreFix)
			/// </summary>
			/// <returns>Iterator Reference</returns>
			Iterator& operator++();
			
			/// <summary>
			/// Reassigns the Iterator to the next node in the list (PostFix)
			/// </summary>
			/// <param name=""></param>
			/// <returns></returns>
			Iterator operator++(int);

			/// <summary>
			/// Dereference operator that returns a reference to the item contained by the node the Iterator points to
			/// </summary>
			/// <returns>T Reference</returns>
			T& operator*() const;

		private:
			Iterator(const SList& owner, Node* node = nullptr);
			
			Node* _node{ nullptr };
			const SList* _owner{ nullptr };
		};

		
		/// <summary>
		/// A class to iterate through an SList
		/// </summary>
		class ConstIterator
		{

			friend SList;
		public:

			ConstIterator() = default;
			ConstIterator(const ConstIterator & other) = default;
			ConstIterator& operator=(const ConstIterator & other) = default;
			~ConstIterator() = default;
			
			/// <summary>
			/// Type-CastConstructor for ConstIterator
			/// </summary>
			/// <param name="other">Takes in a reference to the iterator to be created from</param>
			ConstIterator(const Iterator& other);

			
			/// <summary>
			/// Equal operator that returns true if two iterators point to the same node
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns></returns>
			bool operator==(const ConstIterator& other) const;
			
			/// <summary>
			/// Not Equal operator that returns true if two iterators point to different nodes
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns></returns>
			bool operator!=(const ConstIterator& other) const;

			
			/// <summary>
			/// Reassigns the Iterator to the next node in the list (PreFix)
			/// </summary>
			/// <returns>Iterator Reference</returns>
			ConstIterator& operator++();
			
			/// <summary>
			/// Reassigns the Iterator to the next node in the list (PostFix)
			/// </summary>
			/// <param name=""></param>
			/// <returns></returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Dereference operator that returns a constant reference to the item contained by the node the Iterator points to
			/// </summary>
			/// <returns>T Reference</returns>
			const T& operator*() const;

		private:
			Node* _node{ nullptr };
			const SList* _owner{ nullptr };
		};

		
		/// <summary>
		/// Returns an SList::Iterator pointing to the head of the list.
		/// </summary>
		/// <returns>SList::Iterator</returns>
		Iterator begin();
		
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the head of the list.
		/// </summary>
		/// <returns>SList::ConstIterator</returns>
		ConstIterator begin() const;
		
		
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the head of the list. Const Only
		/// </summary>
		/// <returns>SList::ConstIterator</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an SList::Iterator pointing to the tail of the list.
		/// </summary>
		/// <returns>SList::Iterator</returns>
		Iterator end();
		
		
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the tail of the list.
		/// </summary>
		/// <returns>SList::ConstIterator</returns>
		ConstIterator end() const;
		
		
		/// <summary>
		/// Returns an SList::ConstIterator pointing to the tail of the list. Const Only
		/// </summary>
		/// <returns>SList::ConstIterator</returns>
		ConstIterator cend() const;

		
		/// <summary>
		/// Returns an Iterator pointing to a given item
		/// </summary>
		/// <param name="value"> T Reference that is the item to search for</param>
		/// <returns></returns>
		Iterator Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>());
		
		/// <summary>
		/// Returns an Iterator pointing to a given item (Const Version)
		/// </summary>
		/// <param name="value"> T Reference that is the item to search for</param>
		/// <returns></returns>
		ConstIterator Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>()) const;

		/// <summary>
		/// Append the given item after the item the given iterator points to
		/// </summary>
		/// <param name="value"> Item to be added</param>
		/// <param name="previous"> Previous position in the list where it will be added</param>
		void InsertAfter(const T& value, Iterator previous);

		
		/// <summary>
		/// Remove the item associated with the given data and maintain list integrity
		/// Removes only one instance that occurs first
		/// </summary>
		/// <param name="value"></param>
		void Remove(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>());
	};
}

#include "SList.inl"