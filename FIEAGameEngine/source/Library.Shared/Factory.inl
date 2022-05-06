#include "Factory.h"

namespace FIEAGameEngine
{
	
	template<typename T>
	const Factory<T>* Factory<T>::Find(const std::string className)
	{
		auto it = _factoryList.Find(className);
		if (it != _factoryList.end())
		{
			return (*it).second;
		}

		return nullptr;
	}

	template<typename T>
	gsl::owner <T*> Factory<T>::Create(const std::string className)
	{
		auto factory = Find(className);
		return ((factory == nullptr) ? nullptr : factory->Create());
	}

	template<typename T>
	void Factory<T>::Add(const Factory<T>& factory)
	{
		auto result = _factoryList.Insert(std::make_pair(factory.ClassName(), &factory));

		if (!result.second)
		{
			throw std::runtime_error("Can not add a factory to the list that has already been registered");
		}
	}

	template<typename T>
	void Factory<T>::Remove(const Factory<T>& factory)
	{
		_factoryList.Remove(factory.ClassName());
	}

}