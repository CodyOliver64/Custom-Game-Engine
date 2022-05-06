#include "pch.h"
#include "Datum.h"

namespace FIEAGameEngine
{
	const std::size_t Datum::_datumTypeSizes[] =
	{
		0,
		sizeof(std::int32_t),
		sizeof(float),
		sizeof(glm::vec4),
		sizeof(glm::mat4),
		sizeof(std::string),
		sizeof(RTTI*),
		sizeof(Scope*)
	};
	
	
	
	Datum::Datum(const Datum& other)
	{
		if (other._type == DatumTypes::Unknown) //symptom of lack of move semantics, appending a new datum to a Scope results in the copy of an unknown datum (which errors on SetType). Need proper move semantics in SList/Node
		{
			return;
		}
		
		if (other._isExternal)
		{
			_size = other._size;
			_capacity = other._capacity;
			_type = other._type;
			_data = other._data;
			_isExternal = other._isExternal;
		}
		else
		{
			SetType(other._type);
			Reserve(other._capacity);
			
			if (_type == DatumTypes::String)
			{
				for (size_t i = 0; i < other._size; ++i)
				{
					PushBack(other._data.s[i]);
				}
			}
			else
			{
				std::size_t typeSize = _datumTypeSizes[static_cast<std::size_t>(_type)];
				memcpy(_data.vp, other._data.vp, other._size * typeSize);
				_size = other._size;
			}
		}
	}

	Datum::Datum(Datum&& other) noexcept :
		_size(other._size), _capacity(other._capacity), _type(other._type), _data(other._data), _isExternal(other._isExternal)
	{	
		other._size = 0;
		other._capacity = 0;
		other._data.vp = nullptr;
		other._isExternal = false;
	}
	
	Datum::Datum(DatumTypes type, std::size_t capacity)
	{
		SetType(type);
		Reserve(capacity); 
	}

	Datum::~Datum()
	{
		if (!_isExternal)
		{
			Clear();
			free(_data.vp);
		}
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			if (!_isExternal)
			{
				Clear();
				_capacity = 0;
			}

			if (other._isExternal)
			{
				if (!_isExternal)
				{
					free(_data.vp);
				}
				_data = other._data;
				_size = other._size;
				_capacity = other._capacity;
				_type = other._type;
				_isExternal = true;
			}
			
			if (!other._isExternal)
			{
				if (_type == DatumTypes::Unknown && other._type != DatumTypes::Unknown)
				{
					SetType(other._type);
				}
				Reserve(other._capacity);

				if (_type == DatumTypes::String)
				{
					for (size_t i = 0; i < other._size; ++i)
					{
						PushBack(other._data.s[i]);
					}
				}
				else
				{
					std::size_t typeSize = _datumTypeSizes[static_cast<std::size_t>(_type)];
					_size = other._size;
					memcpy(_data.vp, other._data.vp, _size * typeSize);
				}
			}
		}
		return *this;
	}
	Datum& Datum::operator=(Datum&& other) noexcept
	{
		if (this != &other)
		{
			if (!_isExternal)
			{
				Clear();
				free(_data.vp);
			}

			_size = other._size;
			_capacity = other._capacity;
			_data = other._data;
			_isExternal = other._isExternal;

			other._size = 0;
			other._capacity = 0;
			other._data.vp = nullptr;
		}

		return *this;
	}

	//TypeCast Constructor Overloads
	Datum::Datum(const std::int32_t& value)
	{
		SetType(DatumTypes::Integer);
		Reserve(1);
		PushBack(value);
	}
	Datum::Datum(const float& value)
	{
		SetType(DatumTypes::Float);
		Reserve(1);
		PushBack(value);
	}
	Datum::Datum(const glm::vec4& value)
	{
		SetType(DatumTypes::Vector);
		Reserve(1);
		PushBack(value);
	}
	Datum::Datum(const glm::mat4& value)
	{
		SetType(DatumTypes::Matrix);
		Reserve(1);
		PushBack(value);
	}
	Datum::Datum(const std::string& value)
	{
		SetType(DatumTypes::String);
		Reserve(1);
		PushBack(value);
	}
	Datum::Datum(RTTI* value)
	{
		SetType(DatumTypes::Pointer);
		Reserve(1);
		PushBack(value);
	}

	Datum::Datum(Scope* value)
	{
		SetType(DatumTypes::Table);
		Reserve(1);
		PushBack(value);
	}

	//Scalar Assignment Operators
	Datum& Datum::operator=(const std::int32_t& value)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Integer)
		{
			throw std::runtime_error("Invalid Change of Type");
		}
		if (_isExternal)
		{
			throw std::runtime_error("Invalid operation with external memory");
		}

		Clear();
		PushBack(value);
		if (_capacity > _size)
		{
			ShrinkToFit();
		}

		return *this;
	}
	Datum& Datum::operator=(const float& value)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Float)
		{
			throw std::runtime_error("Invalid Change of Type");
		}

		if (_isExternal && _capacity > 0)
		{
			throw std::runtime_error("Invalid operation with external memory");
		}

		Clear();
		PushBack(value);
		if (_capacity > _size)
		{
			ShrinkToFit();
		}

		return *this;
	}
	Datum& Datum::operator=(const glm::vec4& value)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Vector)
		{
			throw std::runtime_error("Invalid Change of Type");
		}

		if (_isExternal)
		{
			throw std::runtime_error("Invalid operation with external memory");
		}

		Clear();
		PushBack(value);
		if (_capacity > _size)
		{
			ShrinkToFit();
		}

		return *this;
	}
	Datum& Datum::operator=(const glm::mat4& value)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Invalid Change of Type");
		}

		if (_isExternal)
		{
			throw std::runtime_error("Invalid operation with external memory");
		}

		Clear();
		PushBack(value);
		if (_capacity > _size)
		{
			ShrinkToFit();
		}

		return *this;
	}
	Datum& Datum::operator=(const std::string& value)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::String)
		{
			throw std::runtime_error("Invalid Change of Type");
		}

		if (_isExternal)
		{
			throw std::runtime_error("Invalid operation with external memory");
		}

		Clear();
		PushBack(value);
		if (_capacity > _size)
		{
			ShrinkToFit();
		}

		return *this;
	}
	Datum& Datum::operator=(RTTI* value)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Invalid Change of Type");
		}

		if (_isExternal)
		{
			throw std::runtime_error("Invalid operation with external memory");
		}

		Clear();
		PushBack(value);
		if (_capacity > _size)
		{
			ShrinkToFit();
		}

		return *this;
	}
	/*Datum& Datum::operator=(Scope* value) //FIX ME - delete or change to reference instead of ptr. May not need this at all
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Table)
		{
			throw std::runtime_error("Invalid Change of Type");
		}

		if (_isExternal)
		{
			throw std::runtime_error("Invalid operation with external memory");
		}

		Clear();
		PushBack(value);
		if (_capacity > _size)
		{
			ShrinkToFit();
		}

		return *this;
	}*/
	
	
	void Datum::SetType(DatumTypes type)
	{
		if (type == DatumTypes::Unknown || (_type != DatumTypes::Unknown && type != _type))
		{
			throw std::runtime_error("Invalid Change of Type");
		}

		_type = type;
	}

	//PushBack Overloads
	void Datum::PushBack(const std::int32_t& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation on External Storage");
		}
		
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Integer);
		}
		
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}
		
		if (_size == _capacity)
		{
			Reserve((_capacity + 1) * 2);
		}

		_data.i[_size++] = value;

	}
	void Datum::PushBack(const float& value)
	{

		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation on External Storage");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Float);
		}

		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}
		
		if (_size == _capacity)
		{
			Reserve((_capacity + 1) * 2);
		}

		_data.f[_size++] = value;

	}
	void Datum::PushBack(const glm::vec4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation on External Storage");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Vector);
		}

		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		if (_size == _capacity)
		{
			Reserve((_capacity + 1) * 2);
		}

		_data.v[_size++] = value;

	}
	void Datum::PushBack(const glm::mat4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation on External Storage");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Matrix);
		}

		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		if (_size == _capacity)
		{
			Reserve((_capacity + 1) * 2);
		}

		_data.m[_size++] = value;

	}
	void Datum::PushBack(const std::string& value) //string could benefit from move semantics, fix later
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation on External Storage");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::String);
		}

		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		if (_size == _capacity)
		{
			Reserve((_capacity + 1) * 2);
		}

		new(_data.s + _size++)std::string(value);

	}
	void Datum::PushBack(RTTI* value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation on External Storage");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Pointer);
		}

		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		if (_size == _capacity)
		{
			Reserve((_capacity + 1) * 2);
		}

		_data.r[_size++] = value;

	}

	void Datum::PushBack(Scope* value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation on External Storage");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Table);
		}

		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		if (_size == _capacity)
		{
			Reserve((_capacity + 1) * 2);
		}

		_data.t[_size++] = value;

	}

	//Set Overloads
	void Datum::Set(const std::int32_t& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index Passed");
		}

		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		_data.i[index] = value;
	}
	void Datum::Set(const float& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index Passed");
		}
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		_data.f[index] = value;
	}
	void Datum::Set(const glm::vec4& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index Passed");
		}
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		_data.v[index] = value;
	}
	void Datum::Set(const glm::mat4& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index Passed");
		}
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		_data.m[index] = value;
	}
	void Datum::Set(const std::string& value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index Passed");
		}
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		_data.s[index] = value;
	}
	void Datum::Set(RTTI* value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index Passed");
		}
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		_data.r[index] = value;
	}

	void Datum::Set(Scope* value, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index Passed");
		}
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Invalid type match with data and Datum");
		}

		_data.t[index] = value;
	}

	//Get Variants 
	std::int32_t& Datum::GetInt(const size_t index)
	{
		if (index >= _size )
		{
			throw std::runtime_error("Invalid Index");
		}
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}

		return *(_data.i + index);
	}

	float& Datum::GetFloat(const size_t index)
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}

		return *(_data.f + index);
	}

	glm::vec4& Datum::GetVec(const size_t index)
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}

		return *(_data.v + index);
	}

	glm::mat4& Datum::GetMat(const size_t index)
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}

		return *(_data.m + index);
	}

	std::string& Datum::GetString(const size_t index)
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}

		return *(_data.s + index);
	}

	RTTI* Datum::GetPtr(const size_t index)
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}

		return *(_data.r + index);
	}

	Scope* Datum::GetScope(const size_t index)
	{
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}

		return *(_data.t + index);
	}

	//Front Variants 
	std::int32_t& Datum::FrontInt()
	{
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.i);
	}

	float& Datum::FrontFloat()
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.f);
	}

	glm::vec4& Datum::FrontVec()
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.v);
	}

	glm::mat4& Datum::FrontMat()
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.m);
	}

	std::string& Datum::FrontString()
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.s);
	}

	RTTI* Datum::FrontPtr()
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.r);
	}

	//Back Variants 
	std::int32_t& Datum::BackInt()
	{
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.i + (_size - 1));
	}

	float& Datum::BackFloat()
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.f + (_size - 1));
	}

	glm::vec4& Datum::BackVec()
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.v + (_size - 1));
	}

	glm::mat4& Datum::BackMat()
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.m + (_size - 1));
	}

	std::string& Datum::BackString()
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.s + (_size - 1));
	}

	RTTI* Datum::BackPtr()
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Invalid Operation for a datum of this type");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data.r + (_size - 1));
	}

	//Find Overloads
	std::pair<std::size_t, bool> Datum::Find(const std::int32_t& value)
	{
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Passed type does not match Datum type");
		}
		
		size_t index = 0;
		bool found = false;
		
		for (size_t i = 0; i < _size; ++i)
		{
			if (value == _data.i[i])
			{
				index = i;
				found = true;
				break;
			}
		}

		return std::make_pair(index, found);
	}
	std::pair<std::size_t, bool> Datum::Find(const float& value)
	{
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Passed type does not match Datum type");
		}
		
		size_t index = 0;
		bool found = false;

		for (size_t i = 0; i < _size; ++i)
		{
			if (value == _data.f[i])
			{
				index = i;
				found = true;
				break;
			}
		}

		return std::make_pair(index, found);
	}
	std::pair<std::size_t, bool> Datum::Find(const glm::vec4& value)
	{
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Passed type does not match Datum type");
		}
		
		size_t index = 0;
		bool found = false;

		for (size_t i = 0; i < _size; ++i)
		{
			if (value == _data.v[i])
			{
				index = i;
				found = true;
				break;
			}
		}

		return std::make_pair(index, found);
	}
	std::pair<std::size_t, bool> Datum::Find(const glm::mat4& value)
	{
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Passed type does not match Datum type");
		}
		
		size_t index = 0;
		bool found = false;

		for (size_t i = 0; i < _size; ++i)
		{
			if (value == _data.m[i])
			{
				index = i;
				found = true;
				break;
			}
		}

		return std::make_pair(index, found);
	}
	std::pair<std::size_t, bool> Datum::Find(const std::string& value)
	{
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Passed type does not match Datum type");
		}
		
		size_t index = 0;
		bool found = false;

		for (size_t i = 0; i < _size; ++i)
		{
			if (value == _data.s[i])
			{
				index = i;
				found = true;
				break;
			}
		}

		return std::make_pair(index, found);
	}
	std::pair<std::size_t, bool> Datum::Find(RTTI* value)
	{
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Passed type does not match Datum type");
		}
		
		size_t index = 0;
		bool found = false;

		for (size_t i = 0; i < _size; ++i)
		{
			if (value == _data.r[i])
			{
				index = i;
				found = true;
				break;
			}
		}

		return std::make_pair(index, found);
	}

	std::pair<std::size_t, bool> Datum::Find(Scope* value)
	{
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Passed type does not match Datum type");
		}

		size_t index = 0;
		bool found = false;

		for (size_t i = 0; i < _size; ++i)
		{
			if (value == _data.t[i])
			{
				index = i;
				found = true;
				break;
			}
		}

		return std::make_pair(index, found);
	}
	
	
	//Remove Overloads
	bool Datum::Remove(const std::int32_t& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}

		std::pair<std::size_t, bool> status = Find(value);

		if (status.second == true)
		{
			RemoveAt(status.first);
		}

		return status.second;
	}
	bool Datum::Remove(const float& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		std::pair<std::size_t, bool> status = Find(value);

		if (status.second == true)
		{
			RemoveAt(status.first);
		}

		return status.second;
	}
	bool Datum::Remove(const glm::vec4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		std::pair<std::size_t, bool> status = Find(value);

		if (status.second == true)
		{
			RemoveAt(status.first);
		}

		return status.second;
	}
	bool Datum::Remove(const glm::mat4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		std::pair<std::size_t, bool> status = Find(value);

		if (status.second == true)
		{
			RemoveAt(status.first);
		}

		return status.second;
	}
	bool Datum::Remove(const std::string& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		std::pair<std::size_t, bool> status = Find(value);

		if (status.second == true)
		{
			RemoveAt(status.first);
		}

		return status.second;
	}
	bool Datum::Remove(RTTI* value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		std::pair<std::size_t, bool> status = Find(value);

		if (status.second == true)
		{
			RemoveAt(status.first);
		}

		return status.second;
	}


	//SetStorage Overloads
	void Datum::SetStorage(std::int32_t* externalArray, std::size_t size)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can not change to external after allocation");
		}
		if (externalArray == nullptr)
		{
			throw std::runtime_error("External array must not be null");
		}

		SetType(DatumTypes::Integer);
		_data.i = externalArray;
		_size = _capacity = size;
		_isExternal = true;
	}
	void Datum::SetStorage(float* externalArray, std::size_t size)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can not change to external after allocation");
		}
		if (externalArray == nullptr)
		{
			throw std::runtime_error("External array must not be null");
		}

		SetType(DatumTypes::Float);
		_data.f = externalArray;
		_size = _capacity = size;
		_isExternal = true;
	}
	void Datum::SetStorage(glm::vec4* externalArray, std::size_t size)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can not change to external after allocation");
		}
		if (externalArray == nullptr)
		{
			throw std::runtime_error("External array must not be null");
		}

		SetType(DatumTypes::Vector);
		_data.v = externalArray;
		_size = _capacity = size;
		_isExternal = true;
	}
	void Datum::SetStorage(glm::mat4* externalArray, std::size_t size)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can not change to external after allocation");
		}
		if (externalArray == nullptr)
		{
			throw std::runtime_error("External array must not be null");
		}

		SetType(DatumTypes::Matrix);
		_data.m = externalArray;
		_size = _capacity = size;
		_isExternal = true;
	}
	void Datum::SetStorage(std::string* externalArray, std::size_t size)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can not change to external after allocation");
		}
		if (externalArray == nullptr)
		{
			throw std::runtime_error("External array must not be null");
		}

		SetType(DatumTypes::String);
		_data.s = externalArray;
		_size = _capacity = size;
		_isExternal = true;
	}
	void Datum::SetStorage(RTTI** externalArray, std::size_t size)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can not change to external after allocation");
		}
		if (externalArray == nullptr)
		{
			throw std::runtime_error("External array must not be null");
		}

		SetType(DatumTypes::Pointer);
		_data.r = externalArray;
		_size = _capacity = size;
		_isExternal = true;
	}

	void Datum::SetStorage(void* externalArray, std::size_t size)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can not change to external after allocation");
		}
		if (externalArray == nullptr)
		{
			throw std::runtime_error("External array must not be null");
		}

		_data.vp = externalArray;
		_size = _capacity = size;
		_isExternal = true;
	}

	void Datum::SetStorage(Scope* externalScope, std::size_t size)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can not change to external after allocation");
		}
		if (externalScope == nullptr)
		{
			throw std::runtime_error("External array must not be null");
		}

		_data.vp = externalScope;
		_size = _capacity = size;
		_isExternal = true;
	}
	
	
	void Datum::PopBack()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		if (_type == DatumTypes::String)
		{
			(_data.s + (_size - 1))->~basic_string(); // how to destruct a string object
		}
		
		_size--;
	}

	void Datum::Clear()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}

		size_t size = _size;
		for (size_t i = 0; i < size; ++i)
		{
			PopBack();
		}
	}

	bool Datum::RemoveAt(std::size_t index)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		bool removed = false;
		
		if (_size > 0 && index < _size)
		{
			if (_type == DatumTypes::String)
			{
				(_data.s + index)->~basic_string(); // how to destruct a string object
			}

			std::size_t typeSize = _datumTypeSizes[static_cast<std::size_t>(_type)];

			auto dataArray = reinterpret_cast<std::uint8_t*>(_data.vp);

			std::memmove((dataArray + (index * typeSize)), (dataArray + ((index + 1) * typeSize)), (typeSize * (_size - index - 1)));

			--_size;
			removed = true;
		}

		return removed;
	}
	
	void Datum::Reserve(std::size_t capacity)
	{
		
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		if (capacity > _capacity)
		{
			if (_type == DatumTypes::Unknown)
			{
				return;
			}
			std::size_t size = _datumTypeSizes[static_cast<std::size_t>(_type)];
			assert(size != 0);

			void* data = realloc(_data.vp, capacity * size);
			assert(data != nullptr);

			_data.vp = data;
			_capacity = capacity;
		}

	}

	void Datum::Resize(std::size_t size)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		size_t originalSize = _size;
		if (_size > size)
		{
			for (size_t i = size; i < originalSize; ++i)
			{
				PopBack();
			}
		}

		if (_capacity > size)
		{
			ShrinkToFit();
		}
		else
		{
			Reserve(size);

			if (_type == DatumTypes::String)
			{
				for (size_t i = 0; i < (size - originalSize); ++i)
				{
					
					PushBack(std::string()); //example of why i need to addmove/emplace later
				}
			}
			else
			{
				_size = size;
			}
		}
	}

	void Datum::ShrinkToFit()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Invalid Operation for External Storage");
		}
		
		std::size_t size = _datumTypeSizes[static_cast<std::size_t>(_type)];
		assert(size != 0);

		void* data = realloc(_data.vp, _size * size);
		assert(data != nullptr);

		_data.vp = data;
		_capacity = _size;
	}

	std::string Datum::ToString(size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}
		
		switch (_type) { //can fix later with function pointer table

		case DatumTypes::Integer:
			return std::to_string(_data.i[index]);
			break;
		case DatumTypes::Float:
			return std::to_string(_data.f[index]);
			break;
		case DatumTypes::Vector:
			return glm::to_string(_data.v[index]);
			break;
		case DatumTypes::Matrix:
			return glm::to_string(_data.m[index]);
			break;
		case DatumTypes::String:
			return _data.s[index];
			break;
		case DatumTypes::Pointer:
			return _data.r[index]->ToString();
			break;
		default:
			throw std::runtime_error("Can not turn this type into a string");
		}

	}

	void Datum::SetFromString(std::string string, size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index");
		}
		
		switch (_type) {

		case DatumTypes::Integer:
			Set(std::stoi(string), index);
			break;
		case DatumTypes::Float:
			Set(std::stof(string), index);
			break;
		case DatumTypes::Vector:
			float vecFloats[4];
			sscanf_s(string.c_str(), "vec4(%f, %f, %f, %f)", &vecFloats[0], &vecFloats[1], &vecFloats[2], &vecFloats[3]);
			Set(glm::vec4{ vecFloats[0], vecFloats[1], vecFloats[2], vecFloats[3] }, index);
			break;
		case DatumTypes::Matrix:
			float matFloats[16];
			sscanf_s(string.c_str(), "mat4(%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f)", &matFloats[0], &matFloats[1], &matFloats[2], &matFloats[3], &matFloats[4], &matFloats[5], &matFloats[6], &matFloats[7], &matFloats[8], &matFloats[9], &matFloats[10], &matFloats[11], &matFloats[12], &matFloats[13], &matFloats[14], &matFloats[15]);
			Set(glm::mat4{ matFloats[0], matFloats[1], matFloats[2], matFloats[3], matFloats[4], matFloats[5], matFloats[6], matFloats[7], matFloats[8], matFloats[9], matFloats[10], matFloats[11], matFloats[12], matFloats[13], matFloats[14], matFloats[15] }, index);
			break;
		case DatumTypes::String:
			Set(string, index);
			break;
		default:
			throw std::runtime_error("Can not turn this type into a string");
		}
	}

	void Datum::PushBackFromString(std::string string)
	{
		switch (_type) {

		case DatumTypes::Integer:
			PushBack(std::stoi(string));
			break;
		case DatumTypes::Float:
			PushBack(std::stof(string));
			break;
		case DatumTypes::Vector:
			float vecFloats[4];
			sscanf_s(string.c_str(), "vec4(%f, %f, %f, %f)", &vecFloats[0], &vecFloats[1], &vecFloats[2], &vecFloats[3]);
			PushBack(glm::vec4{ vecFloats[0], vecFloats[1], vecFloats[2], vecFloats[3] });
			break;
		case DatumTypes::Matrix:
			float matFloats[16];
			sscanf_s(string.c_str(), "mat4(%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f)", &matFloats[0], &matFloats[1], &matFloats[2], &matFloats[3], &matFloats[4], &matFloats[5], &matFloats[6], &matFloats[7], &matFloats[8], &matFloats[9], &matFloats[10], &matFloats[11], &matFloats[12], &matFloats[13], &matFloats[14], &matFloats[15]);
			PushBack(glm::mat4{ matFloats[0], matFloats[1], matFloats[2], matFloats[3], matFloats[4], matFloats[5], matFloats[6], matFloats[7], matFloats[8], matFloats[9], matFloats[10], matFloats[11], matFloats[12], matFloats[13], matFloats[14], matFloats[15] });
			break;
		case DatumTypes::String:
			PushBack(string);
			break;
		default:
			throw std::runtime_error("Can not turn this type into a string");
		}
	}

	bool Datum::operator==(const Datum& other) const
	{
		bool isSame = true;

		if (_size != other._size || _type != other._type || _isExternal != other._isExternal)
		{
			isSame = false;
		}
		else
		{
			if (_type == DatumTypes::String)  //can use static look up table of function pointers to remove this branch
			{
				for (size_t i = 0; i < _size; ++i)
				{
					if (_data.s[i] != other._data.s[i])
					{
						isSame = false;
					}
				}
			}
			else if (_type == DatumTypes::Pointer || _type == DatumTypes::Table)
			{
				for (size_t i = 0; i < _size; ++i)
				{
					if (!(_data.r[i]->Equals(other._data.r[i])))
					{
						isSame = false;
					}
				}
			}
			else
			{
				std::size_t typeSize = _datumTypeSizes[static_cast<std::size_t>(_type)];
				if (memcmp(_data.vp, other._data.vp, _size * typeSize) != 0)
				{
					isSame = false;
				}
			}
		}
		return isSame;
	}
	bool Datum::operator!=(const Datum& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator==(const std::int32_t& other) const
	{
		bool isSame = true;

		if (_size != 1 || _type != DatumTypes::Integer || _data.i[0] != other)
		{
			isSame = false;
		}
		return isSame;
	}
	bool Datum::operator!=(const std::int32_t& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator==(const float& other) const
	{
		bool isSame = true;

		if (_size != 1 || _type != DatumTypes::Float || _data.f[0] != other)
		{
			isSame = false;
		}
		return isSame;
	}
	bool Datum::operator!=(const float& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator==(const glm::vec4& other) const
	{
		bool isSame = true;

		if (_size != 1 || _type != DatumTypes::Vector || _data.v[0] != other)
		{
			isSame = false;
		}
		return isSame;
	}
	bool Datum::operator!=(const glm::vec4& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator==(const glm::mat4& other) const
	{
		bool isSame = true;

		if (_size != 1 || _type != DatumTypes::Matrix || _data.m[0] != other)
		{
			isSame = false;
		}
		return isSame;
	}
	bool Datum::operator!=(const glm::mat4& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator==(const std::string& other) const
	{
		bool isSame = true;

		if (_size != 1 || _type != DatumTypes::String || _data.s[0] != other)
		{
			isSame = false;
		}
		return isSame;
	}
	bool Datum::operator!=(const std::string& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator==(RTTI* other) const
	{
		bool isSame = true;

		if (_size != 1 || _type != DatumTypes::Pointer || _data.r[0] != other)
		{
			isSame = false;
		}
		return isSame;
	}
	bool Datum::operator!=(RTTI* other) const
	{
		return !(operator==(other));
	}
}