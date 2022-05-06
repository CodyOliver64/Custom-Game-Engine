#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"

#include "JsonParseCoordinator.h"
#include "JsonIntegerParseHelper.h"
#include "JsonStringParseHelper.h"


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
	TEST_CLASS(ParseCoordinatorTests)
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


		TEST_METHOD(Constructor)
		{
			JsonIntegerParseHelper::SharedData sharedData;
			JsonParseCoordinator parser(sharedData);

			Assert::IsFalse(parser.IsClone());
			Assert::AreEqual(size_t(0), parser.GetHelpers().Size());
			Assert::AreEqual(&sharedData, parser.GetSharedData()->As<JsonIntegerParseHelper::SharedData>());

			JsonIntegerParseHelper intHelper;
			parser.AddHelper(intHelper);
			Assert::AreEqual(size_t(1), parser.GetHelpers().Size());

			Assert::AreEqual(&parser, sharedData.GetJsonParseCoordinator());

			JsonIntegerParseHelper::SharedData newSharedData;
			parser.SetSharedData(&newSharedData);
			Assert::AreEqual(&newSharedData, parser.GetSharedData()->As<JsonIntegerParseHelper::SharedData>());
		}

		TEST_METHOD(ParseInt)
		{
			std::string input = R"({"integer": 100})";

			JsonIntegerParseHelper::SharedData sharedData;
			JsonParseCoordinator parser(sharedData);

			JsonIntegerParseHelper intHelper;
			parser.AddHelper(intHelper);

			parser.ParseFromString(input);

			Assert::AreEqual(size_t(1), sharedData._data.Size());
			Assert::AreEqual(100, sharedData._data.Front());
		}

		TEST_METHOD(ParseIntArray)
		{
			std::string input = R"({"integer": [1, 2, 3, 4] })";

			JsonIntegerParseHelper::SharedData sharedData;
			JsonParseCoordinator parser(sharedData);

			JsonIntegerParseHelper intHelper;
			parser.AddHelper(intHelper);

			parser.ParseFromString(input);

			Assert::AreEqual(size_t(4), sharedData._data.Size());
			Assert::AreEqual(1, sharedData._data[0]);
			Assert::AreEqual(2, sharedData._data[1]);
			Assert::AreEqual(3, sharedData._data[2]);
			Assert::AreEqual(4, sharedData._data[3]);
		}

		TEST_METHOD(ParseNestedInts)
		{
			std::string input = R"({
                 "integer": 1,
                 "child" : {
					"integer": [2, 3, 4]
                  }
            })";

			JsonIntegerParseHelper::SharedData sharedData;
			sharedData.Initalize();
			JsonParseCoordinator parser(sharedData);

			JsonIntegerParseHelper intHelper;
			intHelper.Initalize();
			parser.AddHelper(intHelper);

			parser.ParseFromString(input);

			Assert::AreEqual(size_t(4), sharedData._data.Size());
			Assert::AreEqual(2, sharedData._data[0]);
			Assert::AreEqual(3, sharedData._data[1]);
			Assert::AreEqual(4, sharedData._data[2]);
			Assert::AreEqual(1, sharedData._data[3]);

			Assert::AreEqual(size_t(5), intHelper._startHandlerCount);
			Assert::AreEqual(size_t(5), intHelper._endHandlerCount);

			Assert::AreEqual(size_t(2), parser._maxDepth);
			Assert::AreEqual(size_t(0), sharedData.Depth());
		}

		TEST_METHOD(ParseNestedStrings)
		{
			std::string input = R"({
                 "string": "1",
                 "child" : {
					"string": ["2", "3", "4"]
                  }
            })";

			JsonStringParseHelper::SharedData sharedData;
			sharedData.Initalize();
			JsonParseCoordinator parser(sharedData);

			JsonStringParseHelper stringHelper;
			stringHelper.Initalize();
			parser.AddHelper(stringHelper);

			parser.ParseFromString(input);

			Assert::AreEqual(size_t(4), sharedData._data.Size());
			Assert::AreEqual(std::string("2"), sharedData._data[0]);
			Assert::AreEqual(std::string("3"), sharedData._data[1]);
			Assert::AreEqual(std::string("4"), sharedData._data[2]);
			Assert::AreEqual(std::string("1"), sharedData._data[3]);

			Assert::AreEqual(size_t(5), stringHelper._startHandlerCount);
			Assert::AreEqual(size_t(5), stringHelper._endHandlerCount);

			Assert::AreEqual(size_t(2), parser._maxDepth);
			Assert::AreEqual(size_t(0), sharedData.Depth());
		}

		TEST_METHOD(ObjectArray)
		{
			const std::string input = R"(
			{
				"Objects":
				[
					{
						"integer1": 1,
						"integer2": 2,
						"integer3": 3
					},
					{
						"integer4": 4,
						"integer5": 5,
						"integer6": 6
					},
					{
						"integer7": 7,
						"integer8": 8,
						"integer9": 9
					}
				]
			})";

			JsonIntegerParseHelper::SharedData sharedData;
			JsonParseCoordinator parser(sharedData);

			JsonIntegerParseHelper intHelper;
			parser.AddHelper(intHelper);

			parser.ParseFromString(input);

			Assert::AreEqual(size_t(9), sharedData._data.Size());
			Assert::AreEqual(1, sharedData._data[0]);
			Assert::AreEqual(2, sharedData._data[1]);
			Assert::AreEqual(3, sharedData._data[2]);
			Assert::AreEqual(4, sharedData._data[3]);
			Assert::AreEqual(5, sharedData._data[4]);
			Assert::AreEqual(6, sharedData._data[5]);
			Assert::AreEqual(7, sharedData._data[6]);
			Assert::AreEqual(8, sharedData._data[7]);
			Assert::AreEqual(9, sharedData._data[8]);
		}

		TEST_METHOD(MultipleHelpers)
		{
			std::string input = R"({
                 "integer": 1,
                 "child" : {
					"integer": [2, 3, 4]
                  }
            })";

			JsonIntegerParseHelper::SharedData sharedData;
			JsonParseCoordinator parser(sharedData);

			JsonIntegerParseHelper intHelper;
			JsonStringParseHelper stringHelper;
			parser.AddHelper(stringHelper);
			Assert::AreEqual(size_t(1), parser.GetHelpers().Size());

			parser.ParseFromString(input);
			
			Assert::AreEqual(size_t(0), sharedData._data.Size());
			
			parser.AddHelper(intHelper);

			Assert::AreEqual(size_t(2), parser.GetHelpers().Size());

			parser.ParseFromString(input);

			Assert::AreEqual(size_t(4), sharedData._data.Size());
			Assert::AreEqual(2, sharedData._data[0]);
			Assert::AreEqual(3, sharedData._data[1]);
			Assert::AreEqual(4, sharedData._data[2]);
			Assert::AreEqual(1, sharedData._data[3]);

			parser.AddHelper(intHelper);
			Assert::AreEqual(size_t(2), parser.GetHelpers().Size());

			parser.RemoveHelper(stringHelper);
			Assert::AreEqual(size_t(1), parser.GetHelpers().Size());
		}

		TEST_METHOD(Clone)
		{
			JsonIntegerParseHelper::SharedData sharedData;
			JsonParseCoordinator parser(sharedData);

			Assert::IsFalse(parser.IsClone());
			JsonIntegerParseHelper intHelper;
			parser.AddHelper(intHelper);

			auto clone = parser.Clone();
			Assert::AreNotEqual(clone, &parser);
			Assert::IsTrue(clone->IsClone());

			auto helperClone = clone->GetHelpers()[0]->As<JsonIntegerParseHelper>();

			Assert::IsNotNull(helperClone);
			Assert::AreNotEqual(helperClone, parser.GetHelpers()[0]->As<JsonIntegerParseHelper>());
			Assert::AreNotSame(clone->GetSharedData(), parser.GetSharedData());

			JsonIntegerParseHelper badHelper;
			clone->AddHelper(badHelper);
			Assert::AreEqual(size_t(1), clone->GetHelpers().Size());
			clone->RemoveHelper(*helperClone);
			Assert::AreEqual(size_t(1), clone->GetHelpers().Size());

			JsonIntegerParseHelper::SharedData badSharedData;
			Assert::IsFalse(clone->SetSharedData(&badSharedData));

			delete clone;
		}

		TEST_METHOD(ParseFromFile)
		{
			const std::string filename = "TestFile.json";
			std::string input = R"({
                 "integer": 1,
                 "child" : {
					"integer": [2, 3, 4]
                  }
            })";

			std::ofstream outputFile(filename);
			Assert::IsTrue(outputFile.good());

			outputFile << input;
			outputFile.close();

			JsonIntegerParseHelper::SharedData sharedData;
			JsonIntegerParseHelper testParseHelper;
			JsonParseCoordinator parser(sharedData);
			parser.AddHelper(testParseHelper);

			parser.ParseFromFile(filename);

			Assert::AreEqual(filename, parser.GetFileName());

			Assert::AreEqual(size_t(4), sharedData._data.Size());
			Assert::AreEqual(2, sharedData._data[0]);
			Assert::AreEqual(3, sharedData._data[1]);
			Assert::AreEqual(4, sharedData._data[2]);
			Assert::AreEqual(1, sharedData._data[3]);

			Assert::ExpectException<std::exception>([&parser] {parser.ParseFromFile("NonExistentFile.json"); });
		}

		TEST_METHOD(MoveSemantics)
		{
			JsonIntegerParseHelper::SharedData sharedData;
			JsonIntegerParseHelper intHelper;

			{
				JsonParseCoordinator parser(sharedData);
				parser.AddHelper(intHelper);

				JsonParseCoordinator otherParser = std::move(parser);
				Assert::IsFalse(otherParser.IsClone());
				Assert::AreEqual(std::size_t(1), otherParser.GetHelpers().Size());
				Assert::AreEqual(&sharedData, otherParser.GetSharedData()->As<JsonIntegerParseHelper::SharedData>());
				Assert::AreEqual(&otherParser, sharedData.GetJsonParseCoordinator());
			}

			{
				JsonParseCoordinator parser(sharedData);
				parser.AddHelper(intHelper);

				auto clone = parser.Clone();
				
				*clone = std::move(parser);

				Assert::IsFalse(clone->IsClone());
				Assert::AreEqual(std::size_t(1), clone->GetHelpers().Size());
				Assert::AreEqual(&sharedData, clone->GetSharedData()->As<JsonIntegerParseHelper::SharedData>());
				Assert::AreEqual(clone, sharedData.GetJsonParseCoordinator());

				delete clone;
			}
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ParseCoordinatorTests::_startMemState;
}