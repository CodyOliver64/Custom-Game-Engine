#include "DefaultEquality.h"
#include "pch.h"


using namespace std;

namespace FIEAGameEngine
{
	template <typename T>
	bool StandardCompare(const T& value1, const T& value2)
	{
		return std::equal_to<T>(value1, value2);
	}
}
