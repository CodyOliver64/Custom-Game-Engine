#pragma once
#include "DefaultEquality.h"

namespace FIEAGameEngine
{
	template<typename T>
	struct DefaultEquality
	{
		bool operator()(const T& value1, const T& value2) 
		{
			return StandardCompare(value1, value2);
		}
	};

}