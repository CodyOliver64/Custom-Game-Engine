#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
//#include "pch.h"

namespace FIEAGameEngine
{
	template <typename T>
	bool StandardCompare(const T& value1, const T& value2);
}

#include "DefaultEquality.inl"
