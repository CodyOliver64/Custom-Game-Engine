#pragma once

#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"
#include "AttributedFoo.h"
#include "JsonParseCoordinator.h"
#include "JsonIntegerParseHelper.h"
#include "JsonTableParseHelper.h"
#include "GameObject.h"
#include "Event.h"


using namespace FIEAGameEngine;
namespace Microsoft::VisualStudio::CppUnitTestFramework 
{
	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	
	template<typename TKey, typename TData>
	inline std::wstring ToString(const HashMap<TKey, TData>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}
	template<typename TKey, typename TData>
	inline std::wstring ToString( HashMap<TKey, TData>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<typename TKey, typename TData>
	inline std::wstring ToString(const HashMap<TKey, TData>* t)
	{
		RETURN_WIDE_STRING(t->Size());
	}

	template<typename TKey, typename TData>
	inline std::wstring ToString(HashMap<TKey, TData>* t)
	{
		RETURN_WIDE_STRING(t->Size());
	}

	template<typename TKey, typename TData>
	inline std::wstring ToString(const std::pair<TKey, TData>& t)
	{
		try
		{
			return ToString(t.second);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}
	}

	template<typename TKey, typename TData>
	inline std::wstring ToString(const std::pair<TKey, TData>* t)
	{
		try
		{
			return ToString(t->second);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}
	}

	template<typename TKey, typename TData>
	inline std::wstring ToString(std::pair<TKey, TData>& t)
	{
		try
		{
			return ToString(t.second);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}
	}

	template<typename TKey, typename TData>
	inline std::wstring ToString(std::pair<TKey, TData>* t)
	{
		try
		{
			return ToString(t->second);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}
	}
	
	template<>
	inline std::wstring ToString(const HashMap<Foo, Foo>::Iterator& t)
	{
		try
		{
			return ToString((*t).first) + ToString((*t).second);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}

	}

	template<>
	inline std::wstring ToString(const HashMap<Foo, Foo>::ConstIterator& t)
	{
		try
		{
			return ToString((*t).first) + ToString((*t).second);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}

	}

	template<>
	inline std::wstring ToString(const HashMap<int, int>::Iterator& t)
	{
		try
		{
			return ToString((*t).first) + ToString((*t).second);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}

	}

	template<>
	inline std::wstring ToString(const HashMap<int, int>::ConstIterator& t)
	{
		try
		{
			return ToString((*t).first) + ToString((*t).second);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}

	}
	
	
	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}

	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}

	}
	
	
	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}

	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::runtime_error&)
		{
			return L"end()";
		}

	}

	template<>
	inline std::wstring ToString<Foo>(Foo* t)
	{
		RETURN_WIDE_STRING(t->Data());
	}

	template <>
	inline std::wstring ToString<enum DatumTypes>(const enum DatumTypes& t)
	{
		t;
		return L"end()";
	}

	template <>
	inline std::wstring ToString<struct glm::vec<4, float>>(const struct glm::vec<4, float>& t)
	{
		t;
		return L"end()";
	}

	template <>
	inline std::wstring ToString<struct glm::mat<4, 4, float>>(const struct glm::mat<4, 4, float>& t)
	{
		t;
		return L"end()";
	}
	template <>
	inline std::wstring ToString<RTTI>(RTTI* t)
	{
		t;
		return L"end()";
	}

	template <>
	inline std::wstring ToString<Scope>(Scope* t)
	{
		t;
		return L"end()";
	}
	template <>
	inline std::wstring ToString<Datum>(Datum* t)
	{
		t;
		return L"end()";
	}

	template <>
	inline std::wstring ToString<Datum>(const Datum* t)
	{
		t;
		return L"end()";
	}

	template <>
	inline std::wstring ToString<Datum>(const Datum& t)
	{
		t;
		return L"end()";
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Attributed>(Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Attributed>(const Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator>(JsonParseCoordinator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator>(const JsonParseCoordinator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator::SharedData>(JsonParseCoordinator::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<JsonParseCoordinator::SharedData>(const JsonParseCoordinator::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonIntegerParseHelper::SharedData>(JsonIntegerParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<JsonIntegerParseHelper::SharedData>(const JsonIntegerParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}


	template<>
	inline std::wstring ToString<IJsonParseHelper>(IJsonParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonIntegerParseHelper>(JsonIntegerParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTableParseHelper>(JsonTableParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<JsonTableParseHelper::SharedData>(JsonTableParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<JsonTableParseHelper::SharedData>(const JsonTableParseHelper::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<FooFactory>(FooFactory* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<FooFactory>(const FooFactory* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<GameObject>(const GameObject& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<GameObject>(GameObject* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<GameObject>(const GameObject* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Event<Foo>>(Event<Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<EventPublisher>(EventPublisher* t)
	{
		RETURN_WIDE_STRING(t);
	}
}
