#include "pch.h"
#include "JsonTableParseHelper.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper)
	RTTI_DEFINITIONS(JsonTableParseHelper::SharedData)

	
	JsonTableParseHelper::JsonTableParseHelper()
	{
		Initalize();
	}

	void JsonTableParseHelper::Initalize()
	{
		IJsonParseHelper::Initalize();

		_types.Insert(std::make_pair("integer", DatumTypes::Integer));
		_types.Insert(std::make_pair("float", DatumTypes::Float));
		_types.Insert(std::make_pair("vector", DatumTypes::Vector));
		_types.Insert(std::make_pair("matrix", DatumTypes::Matrix));
		_types.Insert(std::make_pair("string", DatumTypes::String));
		_types.Insert(std::make_pair("pointer", DatumTypes::Pointer));
		_types.Insert(std::make_pair("table", DatumTypes::Table));
	}

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
	{
		return new JsonTableParseHelper();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		++_startHandlerCount;

		JsonTableParseHelper::SharedData* data = sharedData.As<JsonTableParseHelper::SharedData>();

		if (data == nullptr)
		{
			return false;
		}

		if (value.isObject())
		{
			_currentIndex = 0;

			if (_contextStack.Size() == 0)
			{
				_contextStack.PushBack(ContextFrame{ "ROOT", data->_rootScope });
			}


			if (key != "class" && key != "value")
			{
				if (key == "Transform")
				{
					_contextStack.PushBack(ContextFrame{ key, _contextStack.Back().currentScope->Find(key)->GetScope() });
				}
				else
				{
					_contextStack.Back().currentScope->Append(key);
					_contextStack.PushBack(ContextFrame{ key, _contextStack.Back().currentScope });
				}
				
			}
			else
			{	
				Datum* datum = _contextStack.Back().currentScope->Find(key);
				if (datum != nullptr && datum->IsExternal())
				{
					_contextStack.PushBack(ContextFrame{ _contextStack.Back().currentKey, datum->GetScope() });
				}
				else
				{
					Scope* newScope = Factory<Scope>::Create(value["class"].asString());
					assert(newScope != nullptr);

					_contextStack.Back().currentScope->Adopt(*newScope, _contextStack.Back().currentKey);
					_contextStack.PushBack(ContextFrame{ _contextStack.Back().currentKey, newScope });
				}
			}
		}
		else
		{
			if (key == "type")
			{
				_contextStack.Back().currentScope->Find(_contextStack.Back().currentKey)->SetType((*_types.Find(value.asString())).second);
				_currentIndex = 0;
			}
			else if (key != "class")
			{
				Datum* currentDatum = _contextStack.Back().currentScope->Find(_contextStack.Back().currentKey);
				if (currentDatum->IsExternal())
				{
					currentDatum->SetFromString(value.asString(), _currentIndex);
					++_currentIndex;
				}
				else
				{
					currentDatum->PushBackFromString(value.asString());
				}
			}
		}

		return true;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		++_endHandlerCount;

		JsonTableParseHelper::SharedData* data = sharedData.As<JsonTableParseHelper::SharedData>();
		
		if (data == nullptr)
		{
			return false;
		}

		if (key != "type" && key != "class")
		{
			if (!(key == "value" && (!value.isObject())))
			{
				_contextStack.PopBack();
			}
		}

		return true;
	}


	//SharedData Definitions
	void JsonTableParseHelper::SharedData::Initalize()
	{
		if (_rootScope == nullptr)
		{
			_rootScope = new Scope();
		}
	}

	JsonTableParseHelper::SharedData::~SharedData()
	{
		delete _rootScope;
	}

	JsonTableParseHelper::SharedData::SharedData(Scope* scope)
	{
		if (scope == nullptr)
		{
			Initalize();
		}
		else
		{
			_rootScope = scope;
		}
	}

	gsl::owner<JsonTableParseHelper::SharedData*> JsonTableParseHelper::SharedData::Create() const
	{
		return new JsonTableParseHelper::SharedData();
	}
}