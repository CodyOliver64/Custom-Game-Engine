#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
}

namespace FIEAGameEngine
{
	template<>
	class DefaultHash<Foo>
	{
	public:
		std::size_t operator()(const Foo& key) const
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(&key.Data()), sizeof(std::int32_t));
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTests)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined (_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined (_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}


		TEST_METHOD(InsertFindInt)
		{
			HashMap<int, int> map;

			int key1 = 1;
			int key2 = 2;
			int val1 = 10;
			int val2 = 20;

			HashMap<int, int>::Iterator it5 = map.Insert(std::make_pair(key1, val1)).first;
			HashMap<int, int>::Iterator it6 = map.Insert(std::make_pair(key2, val2)).first;

			HashMap<int, int>::Iterator it = map.Find(key1);
			Assert::AreEqual(val1, (*it).second);

			HashMap<int, int>::Iterator it2 = map.Find(key2);
			Assert::AreEqual(val2, (*it2).second);

			HashMap<int, int>::Iterator it3 = map.Find(37);
			Assert::AreEqual(map.end(), it3);

			map.Remove(key1);

			HashMap<int, int>::Iterator it8 = map.Find(key1);
			Assert::AreEqual(map.end(), it8);

		}
		
		TEST_METHOD(InsertFindFoo)
		{
			HashMap<Foo, Foo> map;

			Foo key1(1);
			Foo key2(2);
			Foo val1(10);
			Foo val2(20);
			Foo key3(12);
			Foo val3(30);

			HashMap<Foo, Foo>::Iterator nullIt;
			auto expression = [&nullIt]() { nullIt++; };
			auto expression2 = [&nullIt]() { ++nullIt; };
			Assert::ExpectException<std::runtime_error>(expression);
			Assert::ExpectException<std::runtime_error>(expression2);
			
			
			HashMap<Foo, Foo>::Iterator it5 = map.Insert(std::make_pair(key1, val1)).first;
			HashMap<Foo, Foo>::Iterator it10 = it5;
			HashMap<Foo, Foo>::Iterator it6 = map.Insert(std::make_pair(key2, val2)).first;
			map.Insert(std::make_pair(key3, val3));
			map.Insert(std::make_pair(key3, val3));

			
			HashMap<Foo, Foo>::Iterator it15 = it5++;
			HashMap<Foo, Foo>::Iterator it11 = ++it10;

			HashMap<Foo, Foo>::Iterator it = map.Find(key1);
			Assert::AreEqual(val1, (*it).second);

			HashMap<Foo, Foo>::Iterator it2 = map.Find(key2);
			Assert::AreEqual(val2, (*it2).second);

			HashMap<Foo, Foo>::Iterator it3 = map.Find(Foo(37));
			Assert::AreEqual(map.end(), it3);

			map.Remove(key1);

			HashMap<Foo, Foo>::Iterator it8 = map.Find(key1);
			Assert::AreEqual(map.end(), it8);

		}

		TEST_METHOD(Clear_Size)
		{
			HashMap<Foo, Foo> map;

			Assert::AreEqual(size_t(0), map.Size());

			Foo key1(1);
			Foo key2(2);
			Foo val1(10);
			Foo val2(20);
			Foo key3(12);
			Foo val3(30);

			map.Insert(std::make_pair(key1, val1));
			map.Insert(std::make_pair(key2, val2));
			map.Insert(std::make_pair(key3, val3));

			Assert::AreEqual(size_t(3), map.Size());

			map.Clear();

			Assert::AreEqual(size_t(0), map.Size());

			map.Insert(std::make_pair(key1, val1));
			map.Insert(std::make_pair(key2, val2));
			map.Insert(std::make_pair(key3, val3));

			Assert::AreEqual(size_t(3), map.Size());
		}

		TEST_METHOD(ConstIncrement)
		{
			HashMap<Foo, Foo> map;

			Foo key1(3);
			Foo key2(6);
			Foo val1(10);
			Foo val2(20);
			Foo key3(3);
			Foo key4(4);
			Foo val3(30);
			Foo val4(40);
			Foo key5(5);
			Foo key6(6);
			Foo val5(50);
			Foo val6(60);
			
			
			HashMap<Foo, Foo>::ConstIterator nullIt2;
			auto expression3 = [&nullIt2]() { nullIt2++; };
			auto expression4 = [&nullIt2]() { ++nullIt2; };
			Assert::ExpectException<std::runtime_error>(expression3);
			Assert::ExpectException<std::runtime_error>(expression4);
			
			HashMap<Foo, Foo>::ConstIterator it5 = map.Insert(std::make_pair(key1, val1)).first;
			HashMap<Foo, Foo>::ConstIterator it10 = it5;
			HashMap<Foo, Foo>::Iterator it6 = map.Insert(std::make_pair(key2, val2)).first;

			HashMap<Foo, Foo>::ConstIterator it15 = it5++;
			HashMap<Foo, Foo>::ConstIterator it11 = ++it10;

			const HashMap<Foo, Foo> map2;
			HashMap<Foo, Foo>::ConstIterator itX = map2.begin();
			HashMap<Foo, Foo>::ConstIterator itY = map2.end();
		}

		TEST_METHOD(Copying)
		{
			HashMap<Foo, Foo> map;

			Foo key1(3);
			Foo key2(6);
			Foo val1(10);
			Foo val2(20);

			map.Insert(std::make_pair(key1, val1));
			map.Insert(std::make_pair(key2, val2));

			const HashMap<Foo, Foo> map3 = map;
			//HashMap<Foo, Foo>::ConstIterator itZ = map3.begin();
			//Assert::AreEqual(val1, (*itZ).second);
		}

		TEST_METHOD(Rehash)
		{
			HashMap<Foo, Foo> map;

			Foo key1(1);
			Foo key2(2);
			Foo key3(3);
			Foo val1(10);
			Foo val2(20);
			Foo val3(30);




			HashMap<Foo, Foo>::Iterator it1 = map.Insert(std::make_pair(key1, val1)).first;
			HashMap<Foo, Foo>::Iterator it2 = map.Insert(std::make_pair(key2, val2)).first;
			HashMap<Foo, Foo>::Iterator it3 = map.Insert(std::make_pair(key3, val3)).first;

			float load = map.LoadFactor();
			Assert::AreEqual(static_cast<float>(3) / 11 * 100, load);

			Assert::AreEqual(false, map.Rehash(1));

			map.Rehash(23);

		}

		TEST_METHOD(BeginEndEqual)
		{
			HashMap<Foo, Foo> map;
			Assert::AreEqual(map.end(), map.begin());
			Assert::IsFalse(map.end() != map.begin());

			Foo key1(1);
			Foo val1(10);
			map.Insert(std::make_pair(key1, val1));


			HashMap<Foo, Foo>::Iterator it1 = map.begin();
			HashMap<Foo, Foo>::Iterator it2 = map.end();

			Assert::AreNotEqual(map.end(), map.begin());
			Assert::IsTrue(map.end() != map.begin());
		}

		TEST_METHOD(ConstBeginEndEqual)
		{
			HashMap<Foo, Foo> map;
			Assert::AreEqual(map.cend(), map.cbegin());
			Assert::IsFalse(map.cend() != map.cbegin());

			Foo key1(1);
			Foo val1(10);
			const Foo fakeKey(12);

			map.Insert(std::make_pair(key1, val1));


			Assert::AreNotEqual(map.cend(), map.cbegin());
			Assert::IsTrue(map.cend() != map.cbegin());

			const HashMap<Foo, Foo> map2 = map;
			HashMap<Foo, Foo>::ConstIterator it5 = map2.begin();
			Assert::AreEqual(val1, (*it5).second);
			Assert::AreEqual(val1, map2.At(key1));
			Assert::AreEqual(val1, map2[key1]);

			auto expression = [&map2, &fakeKey]() { map2.At(fakeKey); };
			Assert::ExpectException<std::runtime_error>(expression);

			auto expression2 = [&map2, &fakeKey]() { map2[fakeKey]; };
			Assert::ExpectException<std::runtime_error>(expression);
			
		}

		TEST_METHOD(AtContainsBrackets)
		{
			HashMap<Foo, Foo> map;

			const Foo key1(1);
			const Foo key2(2);
			const Foo key3(3);
			const Foo val1(10);
			const Foo val2(20);
			const Foo val3(30);

			const Foo fakeKey(12);

			HashMap<Foo, Foo>::Iterator it1 = map.Insert(std::make_pair(key1, val1)).first;
			HashMap<Foo, Foo>::Iterator it2 = map.Insert(std::make_pair(key2, val2)).first;
			HashMap<Foo, Foo>::Iterator it3 = map.Insert(std::make_pair(key3, val3)).first;

			Assert::AreEqual(true, map.ContainsKey(key1));
			Assert::AreEqual(false, map.ContainsKey(fakeKey));

			Assert::AreEqual(val1, map.At(key1));
			Assert::AreEqual(val1, map[key1]);

			auto expression = [&map, &fakeKey]() { map.At(fakeKey); };
			Assert::ExpectException<std::runtime_error>(expression);

			map[fakeKey];
			Assert::AreEqual(true, map.ContainsKey(fakeKey));
			
		}

		TEST_METHOD(ArrowOp)
		{

			HashMap<Foo, Foo> map;

			const Foo key1(1);
			const Foo key2(2);
			const Foo val1(10);
			const Foo val2(20);


			map.Insert(std::make_pair(key1, val1));
			HashMap<Foo, Foo>::Iterator it1 = map.Insert(std::make_pair(key2, val2)).first;
			HashMap<Foo, Foo>::ConstIterator it2 = map.cbegin();

			Assert::AreEqual(val2, it1->second);
			Assert::AreEqual(val1, it2->second);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState HashMapTests::_startMemState;
}