#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <functional>
#include "DefaultHash.h"
#include "DefaultEquality.h"
#include "Vector.h"
#include "SList.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// HashMap that will take in key/pair values and store them in a Vector of SLists.
	/// This is a template class that can hold any data type, with a provided Default Hash that will need to be modified for objects requiring deep copy
	/// </summary>
	/// <typeparam name="TKey">Key</typeparam>
	/// <typeparam name="TData">Value</typeparam>
	template <typename TKey, typename TData>
	class HashMap final
	{
	public:
		using PairType = std::pair<const TKey, TData>;
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;

	public:
		class Iterator;
		class ConstIterator;

		~HashMap() = default;
		HashMap(const HashMap& other) = default;
		HashMap& operator=(const HashMap& other) = default;
		/// <summary>
		/// Default Move Assignment/Constructor. Moved HashMaps are NOT to be reused
		/// </summary>
		/// <param name="other">Takes in a reference to the other hashmap to be moved</param>
		HashMap(HashMap&& other) = default;
		HashMap& operator=(HashMap&& other) = default;
		
		
		/// <summary>
		/// Default Constructor for hash map that can be customized with passed in parameters
		/// Uses a default hash and equality functor. The default hash supplied will need to be specialized if the data being stored requires a deep copy to remain accurate.
		/// </summary>
		/// <param name="size"> Number of buckets stored in the vector of the map, defaulted to 11. Requires a size of at least 2</param>
		/// <param name="hashFunctor">Default hash function</param>
		/// <param name="equalityFunctor">default equality function</param>
		HashMap(const size_t size = 11, DefaultHash<TKey> hashFunctor = DefaultHash<TKey>(), std::function<bool(const TKey&, const TKey&)> equalityFunctor = std::equal_to<TKey>());

		/// <summary>
		/// Takes in a TKey,TValue pair and attempts to insert it into the list at the appropriate location
		/// If the table already contains this key, the value will not be overwritten
		/// Either way an iterator will be returned that points to the entry
		/// </summary>
		/// <param name="data">TKey TValue pair to be inserted</param>
		/// <returns>Iterator pointing to the inserted item</returns>
		std::pair<Iterator, bool> Insert(const PairType& data);

		
		/// <summary>
		/// Takes in a key and searches for it in the list, returning an iterator that points to the key/value pair found
		/// If not in the list will return end()
		/// </summary>
		/// <param name="value">key to be searched for</param>
		/// <param name="equalityFunctor">optional equality functor</param>
		/// <returns>Iterator that holds the position of the key/value pair</returns>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Const find that takes in a key and searches for it in the list, returning a Const iterator that points to the key/value pair found
		/// If not in the list will return end()
		/// </summary>
		/// <param name="value">key to be searched for</param>
		/// <param name="equalityFunctor">optional equality functor</param>
		/// <returns>ConstIterator that holds the position of the key/value pair</returns>
		ConstIterator Find(const TKey& key) const;

		/// <summary>
		/// Takes in a key and removes the associated pair from the map. Does nothing if the key does not exist 
		/// </summary>
		/// <param name="key">Key to be searched for and removed</param>
		void Remove(const TKey& key);

		/// <summary>
		/// Removes all key/value pairs from the list, leaving the maps capacity intact
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns the number of key/pair values stored in the hash map 
		/// </summary>
		/// <returns>size_t that is # items stored</returns>
		const std::size_t Size() const;

		/// <summary>
		/// Returns a float that represents the percentage of buckets in the map that have been filled (which will result in collision if inserted into)
		/// It is recommended that you rehash the map after about 70% load factor to maintain performance
		/// </summary>
		/// <returns>Returns a float that represents the percentage of nonempty buckets buckets</returns>
		const float LoadFactor() const;

		/// <summary>
		/// Returns a bool that is true if the key is stored in the map, false otherwise
		/// </summary>
		/// <param name="key"></param>
		/// <returns>True (key present) or False</returns>
		const bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Takes a key and returns a reference to the TData stored in the pair that matches the found key.
		/// Does no use bounds checking. If the key is not found, will default construct a new TData and insert it into the map in a pair with the non found key
		/// </summary>
		/// <param name="key">Key to be searched for in the map</param>
		/// <returns>Iterator of the present data or the data that was just inserted</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Takes a key and returns a reference to the TData stored in the pair that matches the found key.
		/// Const version does do bounds checking. Will not insert into the map in the const version
		/// </summary>
		/// <param name="key">Key to be searched for in the map</param>
		/// <returns>Iterator of the present data or end() if not found</returns>
		const TData& operator[](const TKey& key) const;

		/// <summary>
		/// Returns a TData reference at the given key. Uses bounds checking
		/// </summary>
		/// <param name="key">key to be searched for</param>
		/// <returns>TData Reference</returns>
		TData& At(const TKey& key);

		/// <summary>
		/// Returns a const TData reference at the given key. Uses bounds checking
		/// </summary>
		/// <param name="key">key to be searched for</param>
		/// <returns>Const TData Reference</returns>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Function that will take in a new size for the hash map and reinsert each key/pair value into a newly created vector
		/// This will change the hash value for each key, and result in a hash map with increased capacity and decreased chance of collision
		/// The vector is then moved in place.
		/// </summary>
		/// <param name="size">Takes a size to be changed to. Can not be less than 2</param>
		/// <returns>true if rehash was successful, false if not</returns>
		bool Rehash(const size_t size);

	private:
		BucketType _map;
		std::size_t _count{ 0 };

		std::function< size_t(TKey)> _hash{ DefaultHash<TKey>{} };
		std::function<bool(const TKey&, const TKey&)> _equals{ std::equal_to<TKey>() };
	//--------------------------------------------------------------------Iterator Class-----------------------------------------------------------------------------------------------

	public:

		/// <summary>
		/// A class to  iterate and mutate the members of a HashMap
		/// </summary>
		class Iterator
		{
			using PairType = std::pair<const TKey, TData>;
			using ChainType = SList<PairType>;
			using BucketType = Vector<ChainType>;
			friend HashMap;

		public:
			Iterator() = default;
			Iterator(const Iterator& other) = default;
			Iterator& operator=(const Iterator& other) = default;
			~Iterator() = default;

			/// <summary>
			/// Equal operator that returns true if two iterators point to the same index, owner, and SList Iterator
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns>True or False if they point to the same same index, owner, and SList Iterator</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Equal operator that returns false if two iterators point to the same index, owner, and SList Iterator
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns>True or False if they point to the same same index, owner, and SList Iterator</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Reassigns the Iterator to the next index in the list (PreFix)
			/// </summary>
			/// <returns>Iterator Reference</returns>
			Iterator& operator++();

			/// <summary>
			/// Reassigns the Iterator to the next index in the list (PostFix)
			/// </summary>
			/// <param name=""></param>
			/// <returns>Iterator</returns>
			Iterator operator++(int);

			/// <summary>
			/// Dereference operator that returns a reference to the TValue contained at the index the Iterator points to
			/// </summary>
			/// <returns>T Reference</returns>
			PairType& operator*() const;

			/// <summary>
			/// Dereference operator that returns a reference to the TValue contained at the index the Iterator points to
			/// </summary>
			/// <returns>T Reference</returns>
			PairType* operator->() const;

		private:
			Iterator(HashMap& owner, const typename SList<PairType>::Iterator& it, std::size_t index = 0);

			std::size_t _index{ 0 };
			HashMap* _owner{ nullptr };
			typename SList<PairType>::Iterator _chainIT;
		};



		class ConstIterator
		{
			using PairType = std::pair<const TKey, TData>;
			using ChainType = SList<PairType>;
			using BucketType = Vector<ChainType>;
			friend Vector;
		public:

			ConstIterator() = default;
			ConstIterator(const ConstIterator& other) = default;
			ConstIterator& operator=(const ConstIterator& other) = default;
			~ConstIterator() = default;


			/// <summary>
			/// Type-CastConstructor for ConstIterator
			/// </summary>
			/// <param name="other">Takes in a reference to the iterator to be created from</param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// public constructor for ConstIterator that can be used from a non const Hashmap
			/// </summary>
			/// <param name="owner">this hash map</param>
			/// <param name="it">current SList index</param>
			/// <param name="index">current vector index</param>
			ConstIterator(const HashMap& owner, const typename SList<PairType>::ConstIterator& it, std::size_t index = 0);


			/// <summary>
			/// Equal operator that returns true if two const iterators point to the same index, owner, and SList Iterator
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns>True or False if they point to the same same index, owner, and SList Iterator</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Equal operator that returns false if two const iterators point to the same index, owner, and SList Iterator
			/// </summary>
			/// <param name="other">Takes in a constant reference to the other iterator being compared</param>
			/// <returns>True or False if they point to the same same index, owner, and SList Iterator</returns>
			bool operator!=(const ConstIterator& other) const;


			/// <summary>
			/// Reassigns the Iterator to the next index in the list (PreFix)
			/// </summary>
			/// <returns>ConstIterator Reference</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Reassigns the Iterator to the next index in the list (PostFix)
			/// </summary>
			/// <param name=""></param>
			/// <returns>ConstIterator</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Dereference operator that returns a reference to the TValue contained at the index the Iterator points to
			/// </summary>
			/// <returns>T Reference</returns>
			const PairType& operator*() const;

			/// <summary>
			/// Dereference operator that returns a reference to the TValue contained at the index the Iterator points to
			/// </summary>
			/// <returns>T Reference</returns>
			const PairType* operator->() const;

		private:
			
			std::size_t _index{ 0 };
			const HashMap* _owner{ nullptr };
			typename SList<PairType>::ConstIterator _chainIT;
		};

		/// <summary>
		/// Returns an HashMap::Iterator pointing to the first present pair of the map.
		/// </summary>
		/// <returns>HashMap::Iterator</returns>
		Iterator begin();
		/// <summary>
		/// Returns an HashMap::ConstIterator pointing to the first present pair of the map.
		/// </summary>
		/// <returns>HashMap::Iterator</returns>
		ConstIterator begin() const;
		/// <summary>
		/// Returns an HashMap::ConstIterator pointing to the first present pair of the map.
		/// </summary>
		/// <returns>HashMap::Iterator</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an HashMap::Iterator pointing to one after the last element of the list (not in the list).
		/// </summary>
		/// <returns>HashMap::Iterator</returns>
		Iterator end();
		/// <summary>
		/// Returns an HashMap::ConstIterator pointing to one after the last element of the list (not in the list).
		/// </summary>
		/// <returns>HashMap::ConstIterator</returns>
		ConstIterator end() const;
		/// <summary>
		/// Returns an HashMap::ConstIterator pointing to one after the last element of the list (not in the list).
		/// </summary>
		/// <returns>HashMap::ConstIterator</returns>
		ConstIterator cend() const;

	};

}

#include "HashMap.inl"