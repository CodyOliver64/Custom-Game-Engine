#include "Datum.h"

namespace FIEAGameEngine
{
	inline DatumTypes Datum::Type() const
	{
		return _type;
	}

	inline std::size_t Datum::Size() const
	{
		return _size;
	}

	inline std::size_t Datum::Capacity() const
	{
		return _capacity;
	}

	inline bool Datum::IsExternal() const
	{
		return _isExternal;
	}
}
