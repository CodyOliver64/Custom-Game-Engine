#pragma once
#include "DefaultHash.h"

namespace FIEAGameEngine
{
	template<typename T>
	struct DefaultHash
	{
		std::size_t operator()(const T& value)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(&value), sizeof(T));
		}
	};

	template <>
	struct DefaultHash<std::string>  //might need const version of string
	{
		std::size_t operator()(const std::string& value)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(value.c_str()), value.size());
		}
	};

	template <>
	struct DefaultHash<const std::string>  //might need const version of string
	{
		std::size_t operator()(const std::string& value)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(value.c_str()), value.size());
		}
	};

	template <>
	struct DefaultHash<char*>
	{
		
		std::size_t operator()(const char* value)
		{
			size_t seed = 31;
			size_t index = 0;

			while (value[index] != '\0')
			{
				seed += value[index];
				++index;
			}
			return seed;
		}
	};
}