#pragma once
#include "IJsonParseHelper.h"
#include "Scope.h"
#include "Factory.h"


namespace FIEAGameEngine
{
	/// <summary>
	/// This is a helper that is able to De-serialize specific grammar that is outlined in the JsonTest and JsonTest2 Files
	/// Its purpose is to parse nested objects using the ParseCoordinator system while building a Scope that matches the JSON format
	/// This is a helper is intended to be used independently from any other helpers specifically for table parsing into the Scope/Datum system
	/// </summary>
	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper)

	public:

		/// <summary>
		/// A struct that represents a Context Frame on the stack, where the stack maintains our current context through a key and a Scope
		/// This stack will be used to parse nested scopes while keeping track of where nested objects should be appended
		/// </summary>
		struct ContextFrame final
		{
			std::string currentKey;
			Scope* currentScope;
		};
		
		//Embedded Shared Data Class
		class SharedData final : public JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseCoordinator::SharedData)

		public:
			virtual ~SharedData() override;
			
			/// <summary>
			/// Constructor for the shared data that heap allocates memory for the Root Scope
			/// </summary>
			SharedData(Scope* scope = nullptr);
			
			/// <summary>
			/// Allocates the root scope if it has not yet been created, if it has clears the Scope
			/// </summary>
			void Initalize() override;

			/// <summary>
			/// virtual method that can be used to create new SharedData instances
			/// </summary>
			/// <returns>JsonTableParseHelper* to current helper</returns>
			gsl::owner<SharedData*> Create() const override;

			/// <summary>
			/// Root scope that is the data payload in this Shared Data object
			/// </summary>
			Scope* _rootScope{ nullptr };
		};


		/// <summary>
		/// Constructor that populates the _types map, which holds the possible types of our scope.
		/// These types will be required in a appropriate grammar for this helper
		/// </summary>
		JsonTableParseHelper();

		/// <summary>
		/// Called on construction to populate our types and initialize or reset the helper if called manually
		/// </summary>
		void Initalize() override;

		/// <summary>
		/// Start handler for this helper, This will takes in objects that contain a type/value pair to be parsed
		/// This handler will maintain our current context as nested objects are read in, maintaining the stack that holds the target of each append to the scope
		/// The handler will only handle shared data payloads that are defined in this class
		/// </summary>
		/// <param name="sharedData">data payload including the root scope</param>
		/// <param name="key">current key of the object</param>
		/// <param name="value">current value of the object (will handle arrays of objects)</param>
		/// <returns>true if the handler can parse the object</returns>
		bool StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// The end handler will be called for every start handler event
		/// The main purpose of the end handler in this helper is to remove frames from the context stack when appropriate
		/// This includes the handling of arrays of objects
		/// </summary>
		/// <param name="sharedData">data payload including the root scope</param>
		/// <param name="key">current key of the object</param>
		/// <param name="value">current value of the object (will handle arrays of objects)</param>
		/// <returns>true if the handler can parse the object</returns>
		bool EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value) override;

		/// <summary>
		/// virtual method that can be used to create new Helper instances
		/// </summary>
		/// <returns>JsonTableParseHelper* to current helper</returns>
		gsl::owner<IJsonParseHelper*> Create() const override;

		std::size_t _startHandlerCount = 0;
		std::size_t _endHandlerCount = 0;

	private:
		bool _isParsing = false;
		Vector<ContextFrame> _contextStack;
		HashMap<std::string, DatumTypes> _types;

		size_t _currentIndex{ 0 };

	};
}