#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
//#include "pch.h"

namespace FIEAGameEngine
{
	size_t AdditiveHash(const uint8_t* data, size_t size);
}

#include "DefaultHash.inl"