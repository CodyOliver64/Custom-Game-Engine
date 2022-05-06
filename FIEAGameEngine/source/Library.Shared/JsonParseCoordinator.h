#pragma once
#include "gsl/gsl"
#include "RTTI.h"
#include "Vector.h"
#include <fstream>

#pragma warning (push)
#pragma warning (disable:4635)
#pragma warning (disable:26812)
#include <json/json.h>
#pragma warning (pop)

namespace FIEAGameEngine
{
	class IJsonParseHelper;

	
	/// <summary>
	/// This class is a coordinator that handles the JSON De-serialization process for a specific grammar
	/// The coordinators job is to handle the parsing of a given JSON, and is assisted by an IJsonParseHelper which handles the actual De-serialization
	/// The wrapper for the data payload that is loaded has a base implementation defined in the class
	/// </summary>
	class JsonParseCoordinator
	{
		
	public:
		
		/// <summary>
		/// Wrapper class that holds the data payload, this class is to be derived from when building a new helper to manage data
		/// </summary>
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI)

		public:
			friend JsonParseCoordinator;

			/// <summary>
			/// Virtual destructor that should be overloaded to handle the destruction of a helpers shared data that has work to be done on destruction
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// Virtual method that should be overloaded to handle the construction or reset of a helpers shared data that has work to be done on start
			/// </summary>
			virtual void Initalize() {};
			
			/// <summary>
			/// Overridden implementations will create an instance of the shared data. This serves as a virtual constructor
			/// </summary>
			/// <returns>pointer to created instance</returns>
			virtual gsl::owner<SharedData*> Create() const = 0;

			/// <summary>
			/// return the address of the JsonParseCoordinator associated with this object.
			/// </summary>
			/// <returns>JsonParseCoordinator* to current cooridnator</returns>
			JsonParseCoordinator* GetJsonParseCoordinator();

			/// <summary>
			/// Increment a nesting depth counter. This gets incremented upon the start of each element.
			/// </summary>
			void IncrementDepth();
			/// <summary>
			/// Decrement a nesting depth counter. This gets decremented upon the end of each element.
			/// </summary>
			void DecrementDepth();
			
			/// <summary>
			/// Return the current nesting depth.
			/// </summary>
			/// <returns>size_t depth</returns>
			size_t Depth();

		private:
			
			/// <summary>
			/// given the address of an JsonParseCoordinator, assign it to a private member.
			/// </summary>
			/// <param name="coordinator">address of the coordinator to adopt</param>
			void SetJsonParseCoordinator(JsonParseCoordinator* coordinator);


		protected:
			JsonParseCoordinator* _coordinator;
		};

	public:

		/// <summary>
		/// Given a reference to a SharedData object, initialize this object
		/// </summary>
		/// <param name="sharedData">takes in a sharedData to parse data into</param>
		JsonParseCoordinator(SharedData& sharedData);
		
		//Destructor that frees the memory that is claimed when a coordinator is cloned
		~JsonParseCoordinator();

		/// <summary>
		/// Move Constructor that reassigns other Coordinator to the new one. Other Coordinator is NOT to be used after Move is called
		/// </summary>
		/// <param name="other">Other vector to be removed</param>
		JsonParseCoordinator(JsonParseCoordinator&& rhs) noexcept;

		/// <summary>
		/// Move Assignment that clears our current coordinator if necessary and reuse the memory to the left hand side. Other Vector is NOT to be used after Move is called
		/// </summary>
		/// <param name="other">Other vector to be Moved</param>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& rhs) noexcept;

		/// <summary>
		/// Deleted copy semantics. See Clone method for copy
		/// </summary>
		/// <param name="other"></param>
		JsonParseCoordinator(const JsonParseCoordinator& other) = delete;
		/// <summary>
		/// Deleted copy semantics. See Clone method for copy
		/// </summary>
		/// <param name="other"></param>
		JsonParseCoordinator& operator=(const JsonParseCoordinator& other) = delete;

		/// <summary>
		/// Duplicate this object and return its address, a cloned coordinator will have its own heap allocated sharedData and helpers that will need to be deleted on destruction
		/// A cloned coordinator can not change its helpers or its sharedData
		/// </summary>
		/// <returns>pointer to the cloned coordinator</returns>
		gsl::owner<JsonParseCoordinator*> Clone();
		
		/// <summary>
		/// given a reference to an IJsonParseHelper object, add it to the list.
		/// </summary>
		/// <param name="helper">helper to be added to the list</param>
		void AddHelper(IJsonParseHelper& helper);
		
		/// <summary>
		/// given a reference to an IJsonParseHelper object, remove it from the list.
		/// </summary>
		/// <param name="helper">helper to be removed</param>
		void RemoveHelper(IJsonParseHelper& helper);
		
		/// <summary>
		/// Returns a file name if the parser is currently parsing from a file
		/// </summary>
		/// <returns></returns>
		const std::string& GetFileName() const;
		
		/// <summary>
		/// Returns a pointer to the current sharedData adopted by the coordinator
		/// </summary>
		/// <returns>pointer to shared data</returns>
		SharedData* GetSharedData() const;
		const Vector<IJsonParseHelper*>& GetHelpers() const;

		/// <summary>
		/// Returns true/false if the coordinator is a clone
		/// </summary>
		/// <returns>true/false if clone</returns>
		bool IsClone() const;

		/// <summary>
		/// Sets the current sharedData being used by the coordinator
		/// </summary>
		/// <param name="newData"></param>
		/// <returns></returns>
		bool SetSharedData(SharedData* newData);

		/// <summary>
		/// begin the De-serialization process on a JSON string, converting it to an istream to be parsed
		/// </summary>
		/// <param name="string">json string to be parsed</param>
		void ParseFromString(const std::string& string);
		
		/// <summary>
		/// begin the De-serialization process on a JSON file, converting it to an istream to be parsed
		/// </summary>
		/// <param name="string">json file to be parsed</param>
		void ParseFromFile(const std::string& filename);
		
		/// <summary>
		/// Given an istream, feed it into a JSON Value object and begin the De-serialization process
		/// </summary>
		/// <param name="jsonStream">stream to be parsed</param>
		void Parse(std::istream& jsonStream);
		
		size_t _depth;
		size_t _maxDepth;

	private:
		void Parse(const std::string& key, const Json::Value& value, bool isArray);
		void ParseMembers(const Json::Value& root);

		bool isClone{ false };
		Vector<IJsonParseHelper*> _helpers;
		SharedData* _sharedData;
		std::string _filePath;
	};
}

