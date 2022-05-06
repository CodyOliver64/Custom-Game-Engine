#include "pch.h"
#include "JsonIntegerParseHelper.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(JsonIntegerParseHelper)
		RTTI_DEFINITIONS(JsonIntegerParseHelper::SharedData)

		void JsonIntegerParseHelper::Initalize()
	{
		IJsonParseHelper::Initalize();
		_isParsing = false;
	}

	gsl::owner<IJsonParseHelper*> JsonIntegerParseHelper::Create() const
	{
		return new JsonIntegerParseHelper();
	}

	bool JsonIntegerParseHelper::StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		++_startHandlerCount;

		if (value.isObject())
		{
			return true;
		}

		key; //for testing

		JsonIntegerParseHelper::SharedData* data = sharedData.As<JsonIntegerParseHelper::SharedData>(); //this limits the helper to only handle SharedData that it has defined (couldn't use multiple helpers on one shared data)

		/*if (data == nullptr || key != IntegerKey) commented out for testing
		{
			return false;
		}*/

		if (_isParsing)
		{
			throw std::runtime_error("Error - Start command received before finishing current parse");
		}
		_isParsing = true;

		data->_data.PushBack(value.asInt());

		return true;
	}

	bool JsonIntegerParseHelper::EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		++_endHandlerCount;
		key; //for testing
		value;

		JsonIntegerParseHelper::SharedData* data = sharedData.As<JsonIntegerParseHelper::SharedData>();

		if (data == nullptr || _isParsing == false)
		{
			return false;
		}

		_isParsing = false;

		return true;
	}


	//SharedData Definitions
	void JsonIntegerParseHelper::SharedData::Initalize()
	{
		_data.Clear();
		return;
	}

	gsl::owner<JsonIntegerParseHelper::SharedData*> JsonIntegerParseHelper::SharedData::Create() const
	{
		return new JsonIntegerParseHelper::SharedData();
	}
}
