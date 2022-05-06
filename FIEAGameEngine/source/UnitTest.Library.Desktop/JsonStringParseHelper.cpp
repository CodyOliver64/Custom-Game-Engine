#include "pch.h"
#include "JsonStringParseHelper.h"

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(JsonStringParseHelper)
		RTTI_DEFINITIONS(JsonStringParseHelper::SharedData)

		void JsonStringParseHelper::Initalize()
	{
		IJsonParseHelper::Initalize();
		_isParsing = false;
	}

	gsl::owner<IJsonParseHelper*> JsonStringParseHelper::Create() const
	{
		return new JsonStringParseHelper();
	}

	bool JsonStringParseHelper::StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		++_startHandlerCount;

		if (value.isObject())
		{
			return true;
		}

		key; //for testing

		JsonStringParseHelper::SharedData* data = sharedData.As<JsonStringParseHelper::SharedData>(); //this limits the helper to only handle SharedData that it has defined (couldn't use multiple helpers on one shared data)

		if (data == nullptr || key != StringKey)
		{
			return false;
		}

		if (_isParsing)
		{
			throw std::runtime_error("Error - Start command received before finishing current parse");
		}
		_isParsing = true;

		data->_data.PushBack(value.asString());

		return true;
	}

	bool JsonStringParseHelper::EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value)
	{
		++_endHandlerCount;
		value;

		JsonStringParseHelper::SharedData* data = sharedData.As<JsonStringParseHelper::SharedData>();

		if (data == nullptr || key != StringKey || _isParsing == false)
		{
			return false;
		}

		_isParsing = false;

		return true;
	}


	//SharedData Definitions
	void JsonStringParseHelper::SharedData::Initalize()
	{
		_data.Clear();
		return;
	}

	gsl::owner<JsonStringParseHelper::SharedData*> JsonStringParseHelper::SharedData::Create() const
	{
		return new JsonStringParseHelper::SharedData();
	}
}
