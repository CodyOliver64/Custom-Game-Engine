#include "DefaultHash.h"
#include "pch.h"


using namespace std;

namespace FIEAGameEngine
{
	size_t AdditiveHash(const uint8_t* data, size_t size)
	{
		size_t hash = 11;

		for (size_t i = 0; i < size; ++i)
		{
			hash += data[i];
		}

		return hash;
	}
}
