#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <functional>
#include <string>
#include "glm\glm.hpp"
#include "RTTI.h"

#pragma warning (push)
#pragma warning (disable : 4201)
#include "glm/gtx/string_cast.hpp"
#pragma warning (pop)

namespace FIEAGameEngine
{
	enum class DatumTypes
	{
		Unknown,
		Integer,
		Float,
		Vector,
		Matrix,
		String,
		Pointer,
		Table,

		End
	};

	class Scope;
	class Datum final
	{
	public:
		friend class Scope;

		/// <summary>
		/// Defaulted Datum Constructor
		/// </summary>
		Datum() = default;
		/// Default Destructor
		~Datum();

		/// <summary>
		/// Copy Constructor for Datum. Creates a new Datum with its own array in memory that store equivalent data as the datum being copied from
		/// </summary>
		/// <param name="other">Takes in a const Vector to be copied from</param>
		Datum(const Datum& other);

		/// <summary>
		/// Copy Assignment for Datum. Creates a new Datum with its own array in memory that store equivalent data as the datum being copied from
		/// </summary>
		/// <param name="other">Takes in a const Vector to be copied from</param>
		Datum& operator=(const Datum& other);
		/// <summary>
		/// Move Constructor that reassigns other datum to the new one. Other datum is NOT to be used after Move is called
		/// </summary>
		/// <param name="other">Other vector to be removed</param>
		Datum(Datum&& other) noexcept;
		/// <summary>
		/// Move Assignment that reassigns other datum to the new one. Other datum is NOT to be used after Move is called
		/// </summary>
		/// <param name="other">Other vector to be removed</param>
		Datum& operator=(Datum&& other) noexcept;

		Datum(DatumTypes type, std::size_t capacity = 0);

		bool IsExternal() const;
		
		//Type-Cast Constructor Overloads

		/// <summary>
		/// TypeCast Constructor that will set the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum(const std::int32_t& value);
		/// <summary>
		/// TypeCast Constructor that will set the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum(const float& value);
		/// <summary>
		/// TypeCast Constructor that will set the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum(const glm::vec4& value);
		/// <summary>
		/// TypeCast Constructor that will set the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum(const glm::mat4& value);
		/// <summary>
		/// TypeCast Constructor that will set the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum(const std::string& value);
		/// <summary>
		/// TypeCast Constructor that will set the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum(RTTI* value);

		Datum(Scope* value);

		//Scalar Assignment Operators

		/// <summary>
		/// TypeCast Assignment that will resize the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum& operator=(const std::int32_t& value);
		/// <summary>
		/// TypeCast Assignment that will resize the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum& operator=(const float& value);
		/// <summary>
		/// TypeCast Assignment that will resize the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum& operator=(const glm::vec4& value);
		/// <summary>
		/// TypeCast Assignment that will resize the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum& operator=(const glm::mat4& value);
		/// <summary>
		/// TypeCast Assignment that will resize the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum& operator=(const std::string& value);
		/// <summary>
		/// TypeCast Assignment that will resize the datum to be size of 1 using the passed value
		/// </summary>
		/// <param name="value">value to be assigned</param>
		Datum& operator=(RTTI* value);

		//Datum& operator=(Scope* value); //FIX ME - delete or change to reference instead of ptr. May not need this at all

		
		/// <summary>
		/// Returns the current type of the datum
		/// </summary>
		/// <returns></returns>
		DatumTypes Type() const;
		/// <summary>
		/// Sets the type of the datum, datum types can not be reassigned types after construction through normal means
		/// </summary>
		/// <param name="type">datum type to be set</param>
		void SetType(DatumTypes type);

		//PushBack Overloads

		/// <summary>
		/// Appends a new item to the end of the datum array, increasing size and capacity if necessary. Checks type being passed
		/// </summary>
		/// <param name="value">value to be added to the list</param>
		void PushBack(const std::int32_t& value);
		/// <summary>
		/// Appends a new item to the end of the datum array, increasing size and capacity if necessary. Checks type being passed
		/// </summary>
		/// <param name="value">value to be added to the list</param>
		void PushBack(const float& value);
		/// <summary>
		/// Appends a new item to the end of the datum array, increasing size and capacity if necessary. Checks type being passed
		/// </summary>
		/// <param name="value">value to be added to the list</param>
		void PushBack(const glm::vec4& value);
		/// <summary>
		/// Appends a new item to the end of the datum array, increasing size and capacity if necessary. Checks type being passed
		/// </summary>
		/// <param name="value">value to be added to the list</param>
		void PushBack(const glm::mat4& value);
		/// <summary>
		/// Appends a new item to the end of the datum array, increasing size and capacity if necessary. Checks type being passed
		/// </summary>
		/// <param name="value">value to be added to the list</param>
		void PushBack(const std::string& value);
		/// <summary>
		/// Appends a new item to the end of the datum array, increasing size and capacity if necessary. Checks type being passed
		/// </summary>
		/// <param name="value">value to be added to the list</param>
		void PushBack(RTTI* value);


		//Get Variants

		/// <summary>
		/// Returns a reference to the value stored at the passed location (uses index checking, may throw error)
		/// </summary>
		/// <param name="index"> index location to return</param>
		/// <returns>refernece to data type</returns>
		std::int32_t& GetInt(size_t index = 0);
		/// <summary>
		/// Returns a reference to the value stored at the passed location (uses index checking, may throw error)
		/// </summary>
		/// <param name="index"> index location to return</param>
		/// <returns>refernece to data type</returns>
		float& GetFloat(size_t index = 0);
		/// <summary>
		/// Returns a reference to the value stored at the passed location (uses index checking, may throw error)
		/// </summary>
		/// <param name="index"> index location to return</param>
		/// <returns>refernece to data type</returns>
		glm::vec4& GetVec(size_t index = 0);
		/// <summary>
		/// Returns a reference to the value stored at the passed location (uses index checking, may throw error)
		/// </summary>
		/// <param name="index"> index location to return</param>
		/// <returns>refernece to data type</returns>
		glm::mat4& GetMat(size_t index = 0);
		/// <summary>
		/// Returns a reference to the value stored at the passed location (uses index checking, may throw error)
		/// </summary>
		/// <param name="index"> index location to return</param>
		/// <returns>refernece to data type</returns>
		std::string& GetString(size_t index = 0);
		/// <summary>
		/// Returns a reference to the value stored at the passed location (uses index checking, may throw error)
		/// </summary>
		/// <param name="index"> index location to return</param>
		/// <returns>refernece to data type</returns>
		RTTI* GetPtr(size_t index = 0);

		Scope* GetScope(size_t index = 0);

		//Set Overloads

		/// <summary>
		/// Sets the value at the given index using the passed value. Uses type and index checking.
		/// </summary>
		/// <param name="value">value to be appended</param>
		/// <param name="index">index position to modify</param>
		void Set(const std::int32_t& value, size_t index = 0);
		/// <summary>
		/// Sets the value at the given index using the passed value. Uses type and index checking.
		/// </summary>
		/// <param name="value">value to be appended</param>
		/// <param name="index">index position to modify</param>
		void Set(const float& value, size_t index = 0);
		/// <summary>
		/// Sets the value at the given index using the passed value. Uses type and index checking.
		/// </summary>
		/// <param name="value">value to be appended</param>
		/// <param name="index">index position to modify</param>
		void Set(const glm::vec4& value, size_t index = 0);
		/// <summary>
		/// Sets the value at the given index using the passed value. Uses type and index checking.
		/// </summary>
		/// <param name="value">value to be appended</param>
		/// <param name="index">index position to modify</param>
		void Set(const glm::mat4& value, size_t index = 0);
		/// <summary>
		/// Sets the value at the given index using the passed value. Uses type and index checking.
		/// </summary>
		/// <param name="value">value to be appended</param>
		/// <param name="index">index position to modify</param>
		void Set(const std::string& value, size_t index = 0);
		/// <summary>
		/// Sets the value at the given index using the passed value. Uses type and index checking.
		/// </summary>
		/// <param name="value">value to be appended</param>
		/// <param name="index">index position to modify</param>
		void Set(RTTI* value, size_t index = 0);


		//Front Variants

		/// <summary>
		/// Returns a reference to the value stored at the front of the array (uses index checking, may throw error)
		/// </summary>
		/// <returns>refernece to data type</returns>
		std::int32_t& FrontInt();
		/// <summary>
		/// Returns a reference to the value stored at the front of the array (uses index checking, may throw error)
		/// </summary>
		/// <returns>refernece to data type</returns>
		float& FrontFloat();
		/// <summary>
		/// Returns a reference to the value stored at the front of the array (uses index checking, may throw error)
		/// </summary>
		/// <returns>refernece to data type</returns>
		glm::vec4& FrontVec();
		/// <summary>
		/// Returns a reference to the value stored at the front of the array (uses index checking, may throw error)
		/// </summary>
		/// <returns>refernece to data type</returns>
		glm::mat4& FrontMat();
		/// <summary>
		/// Returns a reference to the value stored at the front of the array (uses index checking, may throw error)
		/// </summary>
		/// <returns>refernece to data type</returns>
		std::string& FrontString();
		/// <summary>
		/// Returns a reference to the value stored at the front of the array (uses index checking, may throw error)
		/// </summary>
		/// <returns>refernece to data type</returns>
		RTTI* FrontPtr();

		//Back Variants

		/// <summary>
		/// Returns a reference to the value stored at the back of the array (uses index checking, may throw error)
		/// </summary>
		/// <returns>refernece to data type</returns>
		std::int32_t& BackInt();
		/// <summary>
		/// Returns a reference to the value stored at the back of the array (uses index checking, may throw error)
		/// </summary>
		float& BackFloat();
		/// <summary>
		/// Returns a reference to the value stored at the back of the array (uses index checking, may throw error)
		/// </summary>
		glm::vec4& BackVec();
		/// <summary>
		/// Returns a reference to the value stored at the back of the array (uses index checking, may throw error)
		/// </summary>
		glm::mat4& BackMat();
		/// <summary>
		/// Returns a reference to the value stored at the back of the array (uses index checking, may throw error)
		/// </summary>
		std::string& BackString();
		/// <summary>
		/// Returns a reference to the value stored at the back of the array (uses index checking, may throw error)
		/// </summary>
		RTTI* BackPtr();

		//Find Overloads

		/// <summary>
		/// Locates the value passed in the datum array. returns a bool true if found as well as the index of the array where it was found, false if not
		/// </summary>
		/// <param name="value"> value to be searched for</param>
		/// <returns>returns a pair where first is index and second is bool if found</returns>
		std::pair<std::size_t, bool> Find(const std::int32_t& value);
		/// <summary>
		/// Locates the value passed in the datum array. returns a bool true if found as well as the index of the array where it was found, false if not
		/// </summary>
		/// <param name="value"> value to be searched for</param>
		/// <returns>returns a pair where first is index and second is bool if found</returns>
		std::pair<std::size_t, bool> Find(const float& value);
		/// <summary>
		/// Locates the value passed in the datum array. returns a bool true if found as well as the index of the array where it was found, false if not
		/// </summary>
		/// <param name="value"> value to be searched for</param>
		/// <returns>returns a pair where first is index and second is bool if found</returns>
		std::pair<std::size_t, bool> Find(const glm::vec4& value);
		/// <summary>
		/// Locates the value passed in the datum array. returns a bool true if found as well as the index of the array where it was found, false if not
		/// </summary>
		/// <param name="value"> value to be searched for</param>
		/// <returns>returns a pair where first is index and second is bool if found</returns>
		std::pair<std::size_t, bool> Find(const glm::mat4& value);
		/// <summary>
		/// Locates the value passed in the datum array. returns a bool true if found as well as the index of the array where it was found, false if not
		/// </summary>
		/// <param name="value"> value to be searched for</param>
		/// <returns>returns a pair where first is index and second is bool if found</returns>
		std::pair<std::size_t, bool> Find(const std::string& value);
		/// <summary>
		/// Locates the value passed in the datum array. returns a bool true if found as well as the index of the array where it was found, false if not
		/// </summary>
		/// <param name="value"> value to be searched for</param>
		/// <returns>returns a pair where first is index and second is bool if found</returns>
		std::pair<std::size_t, bool> Find(RTTI* value);

		std::pair<std::size_t, bool> Find(Scope* value);

		//Remove Overloads

		/// <summary>
		/// Removes the value passed by using a Find to determine if it is in the list. Type checking
		/// </summary>
		/// <param name="value">value to be removed</param>
		/// <returns>true if the value was removed</returns>
		bool Remove(const std::int32_t& value);
		/// <summary>
		/// Removes the value passed by using a Find to determine if it is in the list. Type checking
		/// </summary>
		/// <param name="value">value to be removed</param>
		/// <returns>true if the value was removed</returns>
		bool Remove(const float& value);
		/// <summary>
		/// Removes the value passed by using a Find to determine if it is in the list. Type checking
		/// </summary>
		/// <param name="value">value to be removed</param>
		/// <returns>true if the value was removed</returns>
		bool Remove(const glm::vec4& value);
		/// <summary>
		/// Removes the value passed by using a Find to determine if it is in the list. Type checking
		/// </summary>
		/// <param name="value">value to be removed</param>
		/// <returns>true if the value was removed</returns>
		bool Remove(const glm::mat4& value);
		/// <summary>
		/// Removes the value passed by using a Find to determine if it is in the list. Type checking
		/// </summary>
		/// <param name="value">value to be removed</param>
		/// <returns>true if the value was removed</returns>
		bool Remove(const std::string& value);
		/// <summary>
		/// Removes the value passed by using a Find to determine if it is in the list. Type checking
		/// </summary>
		/// <param name="value">value to be removed</param>
		/// <returns>true if the value was removed</returns>
		bool Remove(RTTI* value);


		//SetStorage Overloads
		
		/// <summary>
		/// Sets the storage type to external, where the datum does not own its memory and it instead references an array that is passed in
		/// A datums functionality is limited when it is made external, where it can only modify the array in place
		/// Once set to external, and datum can not be switched. Can not set to external if a datum has already been initalized with values
		/// </summary>
		/// <param name="externalArray">external array to be references</param>
		/// <param name="size">size of external array</param>
		void SetStorage(std::int32_t* externalArray, std::size_t size);
		/// <summary>
		/// Sets the storage type to external, where the datum does not own its memory and it instead references an array that is passed in
		/// A datums functionality is limited when it is made external, where it can only modify the array in place
		/// Once set to external, and datum can not be switched. Can not set to external if a datum has already been initalized with values
		/// </summary>
		/// <param name="externalArray">external array to be references</param>
		/// <param name="size">size of external array</param>
		void SetStorage(float* externalArray, std::size_t size);
		/// <summary>
		/// Sets the storage type to external, where the datum does not own its memory and it instead references an array that is passed in
		/// A datums functionality is limited when it is made external, where it can only modify the array in place
		/// Once set to external, and datum can not be switched. Can not set to external if a datum has already been initalized with values
		/// </summary>
		/// <param name="externalArray">external array to be references</param>
		/// <param name="size">size of external array</param>
		void SetStorage(glm::vec4* externalArray, std::size_t size);
		/// <summary>
		/// Sets the storage type to external, where the datum does not own its memory and it instead references an array that is passed in
		/// A datums functionality is limited when it is made external, where it can only modify the array in place
		/// Once set to external, and datum can not be switched. Can not set to external if a datum has already been initalized with values
		/// </summary>
		/// <param name="externalArray">external array to be references</param>
		/// <param name="size">size of external array</param>
		void SetStorage(glm::mat4* externalArray, std::size_t size);
		/// <summary>
		/// Sets the storage type to external, where the datum does not own its memory and it instead references an array that is passed in
		/// A datums functionality is limited when it is made external, where it can only modify the array in place
		/// Once set to external, and datum can not be switched. Can not set to external if a datum has already been initalized with values
		/// </summary>
		/// <param name="externalArray">external array to be references</param>
		/// <param name="size">size of external array</param>
		void SetStorage(std::string* externalArray, std::size_t size);
		/// <summary>
		/// Sets the storage type to external, where the datum does not own its memory and it instead references an array that is passed in
		/// A datums functionality is limited when it is made external, where it can only modify the array in place
		/// Once set to external, and datum can not be switched. Can not set to external if a datum has already been initalized with values
		/// </summary>
		/// <param name="externalArray">external array to be references</param>
		/// <param name="size">size of external array</param>
		void SetStorage(RTTI** externalArray, std::size_t size);

		void SetStorage(Scope* externalScope, std::size_t size);

		//Comparison Overloads
		
		/// <summary>
		/// Returns true if the datums are equivalent in value and size. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator==(const Datum& other) const;
		/// <summary>
		/// Returns true if the datums are equivalent in value and size. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>false if equal</returns>
		bool operator!=(const Datum& other) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator==(const std::int32_t& other) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>false if equal</returns>
		bool operator!=(const std::int32_t& other) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator==(const float& other) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator!=(const float& other) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator==(const glm::vec4 & value) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator!=(const glm::vec4 & value) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator==(const glm::mat4& value) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator!=(const glm::mat4& value) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator==(const std::string& value) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>false if equal</returns>
		bool operator!=(const std::string& value) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>true if equal</returns>
		bool operator==(RTTI* value) const;
		/// <summary>
		/// Scalar equivalence Returns true if the datum is equivalent in value and size of size 1. Will compare data
		/// </summary>
		/// <param name="other">other value to be compared against</param>
		/// <returns>false if equal</returns>
		bool operator!=(RTTI* value) const;

		/// <summary>
		/// Removes the last item in the datum array, decreasing size and free memory if necessary
		/// </summary>
		void PopBack();

		/// <summary>
		/// Removes all items in the datum array, decreasing size and free memory if necessary
		/// </summary>
		void Clear();
		/// <summary>
		/// Removes the value held in the array at the given index, moving following data into place to keep the array contiguous
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		bool RemoveAt(std::size_t index);
		
		/// <summary>
		/// Returns the current size of the array (number of elements)
		/// </summary>
		/// <returns>number of elements</returns>
		std::size_t Size() const;
		/// <summary>
		/// Returns the current capacity of the array (possible size)
		/// </summary>
		/// <returns>number of elements</returns>
		std::size_t Capacity() const;

		/// <summary>
		/// Reserves memory to hold the passed amount of data
		/// </summary>
		/// <param name="capacity">new capacity</param>
		void Reserve(std::size_t capacity);
		/// <summary>
		/// Resizes the array, destroying objects if the array shrinks and emplacing objects if the array grows
		/// </summary>
		/// <param name="capacity">new capacity</param>
		void Resize(std::size_t capacity);
		/// <summary>
		/// Shrinks the capacity of the array to match the current number of elements
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Returns a string version of the data held at the given index. Uses index checking
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		std::string ToString(size_t index = 0); //need const
		/// <summary>
		/// When passed a string version of the objects desirec, will convert the string and replace the current value at the passed index
		/// </summary>
		/// <param name="string">string to be converted to an object, MUST MATCH string layout</param>
		/// <param name="index">index to place object</param>
		void SetFromString(std::string string, size_t index = 0);

		void PushBackFromString(std::string string);

		inline Scope& operator[](std::size_t index) { return *GetScope(index); }

	private:
		void PushBack(Scope* value);
		void Set(Scope* value, size_t index = 0);

		friend class Attributed;
		void SetStorage(void* externalArray, std::size_t size);
		
		union DatumValues final
		{
			std::int32_t* i;
			float* f;
			glm::vec4* v;
			glm::mat4* m;
			std::string* s;
			RTTI** r;
			Scope** t;
			void* vp{ nullptr }; 
		};

		DatumValues _data;
		std::size_t _capacity{ 0 };
		std::size_t _size{ 0 };
		bool _isExternal{ false };
		DatumTypes _type{ DatumTypes::Unknown };
		static const std::size_t _datumTypeSizes[static_cast<std::size_t>(DatumTypes::End)];
	};
}

#include "Datum.inl"