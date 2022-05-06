#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <functional>

namespace FIEAGameEngine
{
	template <typename T>
	/// <summary>
	/// Vector is dynamically sized template container using an array to hold objects
	/// </summary>
	/// <typeparam name="T">The specified type(T) will dictated what the list will hold</typeparam>
	class Vector final
	{
	public:

		class Iterator;
		class ConstIterator;

		/// <summary>
		/// Vector Constructor. Sets size to zero and sets a default capacity reserving the required memory for the array
		/// </summary>
		Vector(const size_t capacity = 11);
		//Vector Destructor that frees all memory associated with the list after destroying objects in the list
		~Vector();

		Vector(std::initializer_list<T> list);

		/// <summary>
		/// Copy Constructor for Vector. Creates a new Vector with its own array in memory that store equivalent data as the vector being copied from
		/// </summary>
		/// <param name="other">Takes in a const Vector to be copied from</param>
		Vector(const Vector& other);

		/// <summary>
		/// Move Constructor that reassigns other vector to the new one. Other Vector is NOT to be used after Move is called
		/// </summary>
		/// <param name="other">Other vector to be removed</param>
		Vector(Vector&& other) noexcept;

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
		/// Removes and deletes T stored at the back of the list. Does not change capacity
		/// </summary>
		void PopBack();

		/// <summary>
		/// Adds a new T to the list, calls Reserve if the list has reached its capacity at this moment to obtain more memory
		/// </summary>
		/// <param name="data">Data to be held by the new Node</param>
		Iterator PushBack(const T& data);

		
		/// <summary>
		/// Remove the item associated with the given Iterator and maintain list integrity
		/// Removes only one instance at the given index
		/// </summary>
		/// <param name="value">Takes in a T Iterator</param>
		void Remove(Iterator it);
		
		/// <summary>
		/// Remove the item associated with the given data and maintain list integrity
		/// Removes only one instance that occurs first
		/// </summary>
		/// <param name="value">Takes in a T reference to remove, and a equality functor that will be used to compare T's. Default to = equalTo</param>
		void Remove(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>());

		void Remove(const Iterator& it1, const Iterator& it2);

		/// <summary>
		/// Removes all T's in the list calling each destructor, leaving an empty list with size 0. Does not change the capacity
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns true if the list is empty, false if not
		/// </summary>
		/// <returns>True or False</returns>
		bool IsEmpty() const;
		/// <summary>
		/// Returns the current size of the list (how many T's it contains)
		/// </summary>
		/// <returns>size_t Size</returns>
		const std::size_t Size() const;
		/// <summary>
		/// Returns the current capacity of the list (size of memory block array currently held by the vector)
		/// </summary>
		/// <returns>size_t Capacity</returns>
		const std::size_t Capacity() const;

		
		/// <summary>
		/// Reserve will be called whenever an item is added and that item makes it so the size is equal to the capacity
		/// At this point the capacity of the list will double and claim the required memory, throwing an error if the realloc is unsuccessful
		/// Reserve can also be called manually to claim a specific amount of memory
		/// </summary>
		/// <param name="newCapacity"></param>
		void Reserve(const std::size_t newCapacity);

		/// <summary>
		/// Takes in an index and returns a reference to the T held at that index. Uses Index checking to fail if out of bounds
		/// </summary>
		/// <param name="index">index position in the array</param>
		/// <returns>T Reference at given position</returns>
		T& At(const std::size_t index);
		/// <summary>
		/// Takes in an index and returns a const reference to the T held at that index. Uses Index checking to fail if out of bounds
		/// </summary>
		/// <param name="index">index position in the array</param>
		/// <returns>const T Reference at given position</returns>
		const T& At(const std::size_t index) const;

		
		/// <summary>
		/// Takes in a value and searches for it in the list, returning an iterator that holds a pointer to the list and an index location of the given value.
		/// Takes an optional equality functor used to compare T's (default to equalTo)
		/// If not in the list will return end()
		/// </summary>
		/// <param name="value">value to be searched for</param>
		/// <param name="equalityFunctor">optional equality functor</param>
		/// <returns>Iterator containing pointer to the list and an index</returns>
		Iterator Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>());
		/// <summary>
		/// Takes in a value and searches for it in the list, returning an iterator that holds a pointer to the list and an index location of the given value.
		/// Takes an optional equality functor used to compare T's (default to equalTo)
		/// If not in the list will return end()
		/// </summary>
		/// <param name="value">value to be searched for</param>
		/// <param name="equalityFunctor">optional equality functor</param>
		/// <returns>ConstIterator containing pointer to the list and an index</returns>
		ConstIterator Find(const T& value, std::function<bool(const T&, const T&)> equalityFunctor = std::equal_to<T>()) const;
		
		/// <summary>
		/// Assignment Operator - Sets one list equal to another by destroying T's in the list being changed and adding new unique T's that contain equivalent data to the other list
		/// Results in a list of equal size, but does not shrink capacity to match
		/// </summary>
		/// <param name="other">Const reference to the other list to be copied from</param>
		/// <returns>Vector reference</returns>
		Vector& operator=(const Vector& other);

		/// <summary>
		/// Move Assignment that frees all memory associated with the current list reassigns other vector to the vector being changed. Other Vector is NOT to be used after Move is called
		/// </summary>
		/// <param name="other">Other vector to be Moved</param>
		Vector& operator=(Vector&& other) noexcept;

		/// <summary>
		/// Takes in an index and returns a reference to the T held at that index. Uses Index checking to fail if out of bounds
		/// </summary>
		/// <param name="index">index position in the array</param>
		/// <returns>T Reference at given position</returns>
		T& operator[](const std::size_t index);
		/// <summary>
		/// Takes in an index and returns a const reference to the T held at that index. Uses Index checking to fail if out of bounds
		/// </summary>
		/// <param name="index">index position in the array</param>
		/// <returns>Const T Reference at given position</returns>
		const T& operator[](const std::size_t index) const;


		void Resize(std::size_t size);

		Vector operator+(const Vector& other);


	private:

		std::size_t _size{ 0 };
		std::size_t _capacity{ 0 };
		T* _vArray{ nullptr };

		//--------------------------------------------------------------------Iterator Class-----------------------------------------------------------------------------------------------
	public:

		/// <summary>
		/// A class to  iterate and mutate the members of a Vector
		/// </summary>
		class Iterator
		{
			friend Vector;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::bidirectional_iterator_tag;

			Iterator() = default;
			Iterator(const Iterator& other) = default;
			Iterator& operator=(const Iterator& other) = default;
			~Iterator() = default;

			/// <summary>
			/// Equal operator that returns true if two iterators point to the index and Vector
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns>True or False if they point to the same vector/index</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Not Equal operator that returns true if two iterators point to different Vector/index
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns>True or False if they do not point to the same vector/index</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Reassigns the Iterator to the next index in the list (PreFix)
			/// </summary>
			/// <returns>Iterator Reference</returns>
			Iterator& operator++();

			Iterator& operator--();

			/// <summary>
			/// Reassigns the Iterator to the next index in the list (PostFix)
			/// </summary>
			/// <param name=""></param>
			/// <returns>Iterator</returns>
			Iterator operator++(int);

			Iterator operator--(int);

			/// <summary>
			/// Dereference operator that returns a reference to the item contained at the index the Iterator points to
			/// </summary>
			/// <returns>T Reference</returns>
			T& operator*() const;

		private:
			Iterator(const Vector& owner, std::size_t index = 0);

			std::size_t _index{ 0 };
			const Vector* _owner{ nullptr };
		};



		class ConstIterator
		{

			friend Vector;
		public:

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::bidirectional_iterator_tag;

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
			/// Equal operator that returns true if two iterators point to the index and Vector
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns>True or False if they point to the same vector/index</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Not Equal operator that returns true if two iterators point to different Vector/index
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns>True or False if they do not point to the same vector/index</returns>
			bool operator!=(const ConstIterator& other) const;


			/// <summary>
			/// Reassigns the Iterator to the next index in the list (PreFix)
			/// </summary>
			/// <returns>ConstIterator Reference</returns>
			ConstIterator& operator++();

			ConstIterator& operator--();

			/// <summary>
			/// Reassigns the Iterator to the next index in the list (PostFix)
			/// </summary>
			/// <param name=""></param>
			/// <returns>ConstIterator</returns>
			ConstIterator operator++(int);

			ConstIterator operator--(int);

			/// <summary>
			/// Dereference operator that returns a const reference to the item contained at the index the Iterator points to
			/// </summary>
			/// <returns>const T Reference</returns>
			const T& operator*() const;

		private:
			std::size_t _index{ 0 };
			const Vector* _owner{ nullptr };
		};


		/// <summary>
		/// Returns an Vector::Iterator pointing to the first element of the list.
		/// </summary>
		/// <returns>Vector::Iterator</returns>
		Iterator begin();
		/// <summary>
		/// Returns an Vector::ConstIterator pointing to the first element of the list.
		/// </summary>
		/// <returns>Vector::ConstIterator</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Returns an Vector::ConstIterator pointing to the first element of the list. Const only
		/// </summary>
		/// <returns>Vector::ConstIterator</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an Vector::Iterator pointing to one after the last element of the list (not in the list).
		/// </summary>
		/// <returns>Vector::Iterator</returns>
		Iterator end();
		/// <summary>
		/// Returns an Vector::ConstIterator pointing to one after the last element of the list (not in the list).
		/// </summary>
		/// <returns>Vector::ConstIterator</returns>
		ConstIterator end() const;
		/// <summary>
		/// Returns an Vector::ConstIterator pointing to one after the last element of the list (not in the list). Const only
		/// </summary>
		/// <returns>Vector::ConstIterator</returns>
		ConstIterator cend() const;

	};
}

#include "Vector.inl"