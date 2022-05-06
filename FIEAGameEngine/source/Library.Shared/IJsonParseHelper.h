#pragma once
#include "gsl/gsl"
#include "RTTI.h"
#include "JsonParseCoordinator.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// Abstract base class that serves as an interface for all helpers
	/// Helpers will define the data layout of the SharedData and will define the Start/End Handlers that do the actual de-serialization of the json value
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)
	public:
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// Initialize this helper. This will get called just before each file is parsed
		/// </summary>
		virtual void Initalize() {};

		/// <summary>
		/// Given a shared data reference, a string for the Json key, a reference to the Json::Value object, and a bool indicating if the value is an array element, attempt to handle the key/value pair. If this routine does indeed handle the pair, return true, otherwise return false.
		/// </summary>
		/// <param name="sharedData">Pointer to current shared data</param>
		/// <param name="key">json key</param>
		/// <param name="value">json value</param>
		/// <returns>Returns true if this helper is able to handle the data</returns>
		virtual bool StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value) = 0;
		
		/// <summary>
		/// Given a shared data reference, a string for the Json key, attempt to complete the handling of the element pair. If this routine does indeed handle the pair, return true, otherwise return false. 
		/// </summary>
		/// <param name="sharedData">Pointer to current shared data</param>
		/// <param name="key">json key</param>
		/// <returns>Returns true if this helper is able to finish handling the data</returns>
		virtual bool EndHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& value) = 0;
		
		/// <summary>
		/// return the address of the JsonParseCoordinator associated with this object.
		/// </summary>
		/// <returns>JsonParseCoordinator* to current cooridnator</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;
	};
}
