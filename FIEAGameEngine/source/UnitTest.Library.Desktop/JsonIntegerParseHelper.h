#pragma once
#include "IJsonParseHelper.h"


namespace FIEAGameEngine
{
	class JsonIntegerParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerParseHelper, IJsonParseHelper)

	public:

		//Embedded Shared Data Class
		class SharedData final : public JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseCoordinator::SharedData)

		public:
			void Initalize() override;
			gsl::owner<SharedData*> Create() const override;

			Vector<std::int32_t> _data;
		};


		void Initalize() override;

		bool StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value) override;
		bool EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value) override;

		gsl::owner<IJsonParseHelper*> Create() const override;

		std::size_t _startHandlerCount = 0;
		std::size_t _endHandlerCount = 0;

	private:
		//inline static const std::string IntegerKey = "integer";
		bool _isParsing = false;
	};
}


