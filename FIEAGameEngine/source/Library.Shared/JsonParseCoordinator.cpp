#include "pch.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(JsonParseCoordinator::SharedData)

	JsonParseCoordinator::JsonParseCoordinator(SharedData& sharedData)
	{
		sharedData.SetJsonParseCoordinator(this);
		//sharedData.Initalize();
		SetSharedData(&sharedData);
	}
	JsonParseCoordinator::~JsonParseCoordinator()
	{
		if (isClone)
		{
			for (size_t i = 0; i < _helpers.Size(); ++i)
			{
				delete _helpers[i];
			}

			delete _sharedData;
		}
	}

	JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& rhs) noexcept :
		_helpers(std::move(rhs._helpers)), _sharedData(rhs._sharedData), isClone(rhs.isClone)
	{
		rhs.GetSharedData()->_coordinator = this;

		rhs._helpers.Clear();
		rhs._sharedData = nullptr;
		rhs.isClone = false;
	}

	JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& rhs) noexcept
	{
		if (this != &rhs)
		{
			if (isClone)
			{
				for (size_t i = 0; i < _helpers.Size(); ++i)
				{
					delete _helpers[i];
				}

				delete _sharedData;
			}

			_helpers = std::move(rhs._helpers);
			_sharedData = rhs._sharedData;
			isClone = rhs.isClone;

			rhs.GetSharedData()->_coordinator = this;

			rhs._helpers.Clear();
			rhs._sharedData = nullptr;
			rhs.isClone = false;
		}
		return *this;
	}
	
	gsl::owner<JsonParseCoordinator*> JsonParseCoordinator::Clone()
	{
		
		SharedData* newData = _sharedData->Create();
		JsonParseCoordinator* newClone = new JsonParseCoordinator(*newData);

		for (size_t i = 0; i < _helpers.Size(); ++i)
		{
			newClone->AddHelper(*(_helpers[i]->Create()));
		}

		newClone->isClone = true;

		return newClone;
	}

	
	void JsonParseCoordinator::ParseFromString(const std::string& string)
	{
		std::stringstream stream;
		stream << string;
		Parse(stream);
	}

	void JsonParseCoordinator::ParseFromFile(const std::string& filename)
	{
		std::ifstream fileStream(filename, std::ifstream::binary);
		_filePath = filename;
		Parse(fileStream);
	}

	void JsonParseCoordinator::Parse(std::istream& stream)
	{
		Json::Value root;
		stream >> root;
		ParseMembers(root);
	}

	void JsonParseCoordinator::ParseMembers(const Json::Value& root)
	{
		const auto memberNames = root.getMemberNames();

		_sharedData->IncrementDepth();

		for (const auto& memberName : memberNames)
		{
			Parse(memberName, root[memberName], root[memberName].isArray());
		}

		_sharedData->DecrementDepth();
	}
	
	void JsonParseCoordinator::Parse(const std::string& key, const Json::Value& value, bool isArray)
	{
		if (isArray)
		{
			for (Json::Value::ArrayIndex i = 0; i < value.size(); ++i)
			{
				//loop through our Helpers, calling start handlers until one accepts the grammar - MAKE FUNCTION
				for (size_t j = 0; j < _helpers.Size(); ++j)
				{
					if (_helpers[j]->StartHandler(*_sharedData, key, value[i]))
					{
						if (value[i].isObject())
						{
							ParseMembers(value[i]);
						}

						_helpers[j]->EndHandler(*_sharedData, key, value[i]);
						break;
					}
				}
			}
		}
		else
		{
			//loop through our Helpers, calling start handlers until one accepts the grammar - MAKE FUNCTION
			for (size_t i = 0; i < _helpers.Size(); ++i)
			{
				if (_helpers[i]->StartHandler(*_sharedData, key, value))
				{
					if (value.isObject())
					{
						ParseMembers(value);
					}

					_helpers[i]->EndHandler(*_sharedData, key, value);
					break;
				}
			}
		}
	}
	
	void JsonParseCoordinator::AddHelper(IJsonParseHelper& helper)
	{
		if (!isClone)
		{
			for (size_t i = 0; i < _helpers.Size(); ++i)
			{
				if (_helpers[i]->TypeIdInstance() == helper.TypeIdInstance())
				{
					return;
				}
			}
			_helpers.PushBack(&helper);
		}
	}
	void JsonParseCoordinator::RemoveHelper(IJsonParseHelper& helper)
	{
		if (!isClone)
		{
			_helpers.Remove(&helper);
		}
	}
	
	const std::string& JsonParseCoordinator::GetFileName() const
	{
		return _filePath;
	}

	JsonParseCoordinator::SharedData* JsonParseCoordinator::GetSharedData() const
	{
		return _sharedData;
	}

	const Vector<IJsonParseHelper*>& JsonParseCoordinator::GetHelpers() const
	{
		return _helpers;
	}

	bool JsonParseCoordinator::IsClone() const
	{
		return isClone;
	}

	bool JsonParseCoordinator::SetSharedData(SharedData* newData)
	{
		if (!isClone)
		{
			_sharedData = newData;
			return true;
		}
		return false;
	}
	
	
	
	// Base SharedData Definitions
	JsonParseCoordinator* JsonParseCoordinator::SharedData::GetJsonParseCoordinator()
	{
		return _coordinator;
	}

	void JsonParseCoordinator::SharedData::SetJsonParseCoordinator(JsonParseCoordinator* coordinator)
	{
		_coordinator = coordinator;
	}

	void JsonParseCoordinator::SharedData::IncrementDepth()
	{
		++_coordinator->_depth;

		if (_coordinator->_depth > _coordinator->_maxDepth)
		{
			_coordinator->_maxDepth = _coordinator->_depth;
		}
	}

	void JsonParseCoordinator::SharedData::DecrementDepth()
	{
		--_coordinator->_depth;
	}

	size_t JsonParseCoordinator::SharedData::Depth()
	{
		return _coordinator->_depth;
	}
}
