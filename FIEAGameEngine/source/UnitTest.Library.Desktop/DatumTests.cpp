#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "DefaultHash.h"
#include "ToStringSpecializations.h"
#include "Datum.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FIEAGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTests)
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


		TEST_METHOD(Constructors)
		{
			{
				Datum d;
				Assert::AreEqual(size_t(0), d.Size());
				Assert::AreEqual(size_t(0), d.Capacity());
				Assert::AreEqual(DatumTypes::Unknown, d.Type());
			}
			{
				Datum d(DatumTypes::Vector);
				Assert::AreEqual(size_t(0), d.Size());
				Assert::AreEqual(size_t(0), d.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d.Type());
			}
			{
				size_t capacity = 10;
				Datum d(DatumTypes::String, capacity);
				Assert::AreEqual(size_t(0), d.Size());
				Assert::AreEqual(capacity, d.Capacity());
				Assert::AreEqual(DatumTypes::String, d.Type());
			}
		}
		TEST_METHOD(TypeCastConstructors)
		{
			{
				int32_t num = 5;
				Datum d = num;

				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(size_t(1), d.Capacity());
				Assert::AreEqual(DatumTypes::Integer, d.Type());
				Assert::AreEqual(num, d.GetInt());
			}

			{
				float num = 5.0;
				Datum d = num;

				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(size_t(1), d.Capacity());
				Assert::AreEqual(DatumTypes::Float, d.Type());
				Assert::AreEqual(num, d.GetFloat());
			}

			{
				glm::vec4 num = { 1.1, 2.2, 3.3, 4.4 };
				Datum d = num;

				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(size_t(1), d.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d.Type());
				Assert::AreEqual(num, d.GetVec());
			}

			{
				glm::mat4 num = { 1.1, 2.2, 3.3, 4.4, 1.1, 2.2, 3.3, 4.4, 1.1, 2.2, 3.3, 4.4, 1.1, 2.2, 3.3, 4.4 };
				Datum d = num;

				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(size_t(1), d.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, d.Type());
				Assert::AreEqual(num, d.GetMat());
			}

			{
				std::string num = "string";
				Datum d = num;

				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(size_t(1), d.Capacity());
				Assert::AreEqual(DatumTypes::String, d.Type());
				Assert::AreEqual(num, d.GetString());
			}

			{
				Foo num = 5;
				RTTI* ptr = &num;
				Datum d = ptr;

				Assert::AreEqual(size_t(1), d.Size());
				Assert::AreEqual(size_t(1), d.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, d.Type());
				Assert::AreEqual(ptr, d.GetPtr());
			}
		}

		TEST_METHOD(AssignmentOverloads)
		{
			{
				Datum datum;
				int32_t a = 1;
				int32_t b = 2;
				int32_t c = 3;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				int32_t d = 4;

				datum = d;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(d, datum.GetInt());

				
				Datum datum2(DatumTypes::Vector);
				auto expression = [&datum2, &d]() { datum2 = d; };
				//Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum2]() { datum2.SetType(DatumTypes::Vector); };
				//Assert::ExpectException<std::runtime_error>(expression2);

				//std::string hi("hi");
				//auto expression3 = [&datum, &hi]() { datum.PushBack(hi); };
				//Assert::ExpectException<std::runtime_error>(expression3);
			}

			{
				Datum datum;
				float a = 1;
				float b = 2;
				float c = 3;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				float d = 4;

				datum = d;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Float, datum.Type());
				Assert::AreEqual(d, datum.GetFloat());


				Datum datum2(DatumTypes::Vector);
				auto expression = [&datum2, &d]() { datum2 = d; };
				Assert::ExpectException<std::runtime_error>(expression);

				int32_t hi = 1;
				auto expression3 = [&datum, &hi]() { datum.PushBack(hi); };
				Assert::ExpectException<std::runtime_error>(expression3);
			}

			{
				Datum datum;
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 b = glm::vec4{ 2,2,3,4 };
				glm::vec4 c = glm::vec4{ 3,2,3,4 };

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				glm::vec4 d = glm::vec4{ 4,2,3,4 };

				datum = d;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
				Assert::AreEqual(d, datum.GetVec());


				Datum datum2(DatumTypes::Integer);
				auto expression = [&datum2, &d]() { datum2 = d; };
				Assert::ExpectException<std::runtime_error>(expression);

				float hi = 1;
				auto expression3 = [&datum, &hi]() { datum.PushBack(hi); };
				Assert::ExpectException<std::runtime_error>(expression3);
			}

			{
				Datum datum;
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 b = glm::mat4{ 2,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 c = glm::mat4{ 3,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				glm::mat4 d = glm::mat4{ 4,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				datum = d;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(d, datum.GetMat());


				Datum datum2(DatumTypes::Integer);
				auto expression = [&datum2, &d]() { datum2 = d; };
				Assert::ExpectException<std::runtime_error>(expression);

				RTTI* hi = nullptr;
				auto expression3 = [&datum, hi]() { datum.PushBack(hi); };
				Assert::ExpectException<std::runtime_error>(expression3);
			}

			{
				Datum datum;
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				std::string d = "4";

				datum = d;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual(d, datum.GetString());


				Datum datum2(DatumTypes::Vector);
				auto expression = [&datum2, &d]() { datum2 = d; };
				Assert::ExpectException<std::runtime_error>(expression);

				glm::mat4 hi = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				auto expression3 = [&datum, &hi]() { datum.PushBack(hi); };
				Assert::ExpectException<std::runtime_error>(expression3);
			}

			{
				Datum datum;
				Foo foo1 = 1;
				Foo foo2 = 2;
				Foo foo3 = 3;
				Foo foo4 = 4;
				RTTI* a = &foo1;
				RTTI* b = &foo2;
				RTTI* c = &foo3;
				RTTI* d = &foo4;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);


				datum = d;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(d, datum.GetPtr());


				Datum datum2(DatumTypes::Vector);
				auto expression = [&datum2, &d]() { datum2 = d; };
				Assert::ExpectException<std::runtime_error>(expression);

				glm::vec4 hi = glm::vec4{ 1,2,3,4 };
				auto expression3 = [&datum, &hi]() { datum.PushBack(hi); };
				Assert::ExpectException<std::runtime_error>(expression3);
			}
		}

		TEST_METHOD(SetOverloads)
		{
			{
				Datum datum;
				int32_t a = 1;
				int32_t b = 2;
				int32_t c = 3;
				int32_t d = 4;
				int32_t e = 5;


				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 1);
				Assert::AreEqual(d, datum.GetInt(1));

				auto expression = [&datum, &e]() { datum.Set(e, 7); };
				Assert::ExpectException<std::runtime_error>(expression);

				float test = 1;
				auto expression2 = [&datum, &test]() { datum.Set(test, 1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				float a = 1;
				float b = 2;
				float c = 3;
				float d = 4;
				float e = 5;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 1);
				Assert::AreEqual(d, datum.GetFloat(1));

				auto expression = [&datum, &e]() { datum.Set(e, 7); };
				Assert::ExpectException<std::runtime_error>(expression);

				glm::vec4 test = glm::vec4{ 1,2,3,4 };
				auto expression2 = [&datum, &test]() { datum.Set(test, 1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 b = glm::vec4{ 2,2,3,4 };
				glm::vec4 c = glm::vec4{ 3,2,3,4 };
				glm::vec4 d = glm::vec4{ 4,2,3,4 };
				glm::vec4 e = glm::vec4{ 5,2,3,4 };

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 1);
				Assert::AreEqual(d, datum.GetVec(1));

				auto expression = [&datum, &e]() { datum.Set(e, 7); };
				Assert::ExpectException<std::runtime_error>(expression);

				glm::mat4 test = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				auto expression2 = [&datum, &test]() { datum.Set(test, 1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 b = glm::mat4{ 2,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 c = glm::mat4{ 3,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 d = glm::mat4{ 4,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 e = glm::mat4{ 5,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 1);
				Assert::AreEqual(d, datum.GetMat(1));

				auto expression = [&datum, &e]() { datum.Set(e, 7); };
				Assert::ExpectException<std::runtime_error>(expression);

				std::string test = "5";
				auto expression2 = [&datum, &test]() { datum.Set(test, 1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";
				std::string d = "4";
				std::string e = "5";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 1);
				Assert::AreEqual(d, datum.GetString(1));

				auto expression = [&datum, &e]() { datum.Set(e, 7); };
				Assert::ExpectException<std::runtime_error>(expression);

				Foo foo1 = 1;
				RTTI* test = &foo1;
				auto expression2 = [&datum, test]() { datum.Set(test, 1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				Foo foo1 = 1;
				Foo foo2 = 2;
				Foo foo3 = 3;
				Foo foo4 = 4;
				Foo foo5 = 5;
				RTTI* a = &foo1;
				RTTI* b = &foo2;
				RTTI* c = &foo3;
				RTTI* d = &foo4;
				RTTI* e = &foo5;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 1);
				Assert::AreEqual(d, datum.GetPtr(1));

				auto expression = [&datum, &e]() { datum.Set(e, 7); };
				Assert::ExpectException<std::runtime_error>(expression);

				int32_t test = 3;
				auto expression2 = [&datum, test]() { datum.Set(test, 1); };
				Assert::ExpectException<std::runtime_error>(expression2);

			}
		}

		TEST_METHOD(GetOverloads)
		{
			{
				Datum datum;
				int32_t a = 1;
				int32_t b = 2;
				int32_t c = 3;
				int32_t e = 5;


				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto expression = [&datum, &e]() { datum.GetInt(7); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { datum.GetFloat(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				float a = 1;
				float b = 2;
				float c = 3;

				float e = 5;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto expression = [&datum, &e]() { datum.GetFloat(7); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { datum.GetVec(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 b = glm::vec4{ 2,2,3,4 };
				glm::vec4 c = glm::vec4{ 3,2,3,4 };

				glm::vec4 e = glm::vec4{ 5,2,3,4 };

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto expression = [&datum, &e]() { datum.GetVec(7); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { datum.GetMat(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 b = glm::mat4{ 2,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 c = glm::mat4{ 3,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				glm::mat4 e = glm::mat4{ 5,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto expression = [&datum, &e]() { datum.GetMat(7); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { datum.GetString(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				std::string e = "5";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto expression = [&datum, &e]() { datum.GetString(7); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { datum.GetPtr(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum;
				Foo foo1 = 1;
				Foo foo2 = 2;
				Foo foo3 = 3;

				Foo foo5 = 5;
				RTTI* a = &foo1;
				RTTI* b = &foo2;
				RTTI* c = &foo3;

				RTTI* e = &foo5;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto expression = [&datum, &e]() { datum.GetPtr(7); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { datum.GetInt(1); };
				Assert::ExpectException<std::runtime_error>(expression2);

			}
		}

		TEST_METHOD(FrontOverloads)
		{
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 1;
				int32_t b = 2;
				int32_t c = 3;
				int32_t e = 5;


				auto expression = [&datum, &e]() { datum.FrontInt(); };
				Assert::ExpectException<std::runtime_error>(expression);
				
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(a, datum.FrontInt());

				auto expression2 = [&datum]() { datum.FrontFloat(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Float);
				float a = 1;
				float b = 2;
				float c = 3;

				float e = 5;

				auto expression = [&datum, &e]() { datum.FrontFloat(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(a, datum.FrontFloat());

				auto expression2 = [&datum]() { datum.FrontVec(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Vector);
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 b = glm::vec4{ 2,2,3,4 };
				glm::vec4 c = glm::vec4{ 3,2,3,4 };

				glm::vec4 e = glm::vec4{ 5,2,3,4 };

				auto expression = [&datum, &e]() { datum.FrontVec(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(a, datum.FrontVec());

				auto expression2 = [&datum]() { datum.FrontMat(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Matrix);
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 b = glm::mat4{ 2,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 c = glm::mat4{ 3,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				glm::mat4 e = glm::mat4{ 5,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				auto expression = [&datum, &e]() { datum.FrontMat(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(a, datum.FrontMat());

				auto expression2 = [&datum]() { datum.FrontString(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::String);
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				std::string e = "5";

				auto expression = [&datum, &e]() { datum.FrontString(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(a, datum.FrontString());

				auto expression2 = [&datum]() { datum.FrontPtr(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Pointer);
				Foo foo1 = 1;
				Foo foo2 = 2;
				Foo foo3 = 3;

				Foo foo5 = 5;
				RTTI* a = &foo1;
				RTTI* b = &foo2;
				RTTI* c = &foo3;

				RTTI* e = &foo5;

				auto expression = [&datum, &e]() { datum.FrontPtr(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(a, datum.FrontPtr());

				auto expression2 = [&datum]() { datum.FrontInt(); };
				Assert::ExpectException<std::runtime_error>(expression2);

			}
		}

		TEST_METHOD(BackOverloads)
		{
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 1;
				int32_t b = 2;
				int32_t c = 3;
				int32_t e = 5;


				auto expression = [&datum, &e]() { datum.BackInt(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(c, datum.BackInt());

				auto expression2 = [&datum]() { datum.BackFloat(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Float);
				float a = 1;
				float b = 2;
				float c = 3;

				float e = 5;

				auto expression = [&datum, &e]() { datum.BackFloat(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(c, datum.BackFloat());

				auto expression2 = [&datum]() { datum.BackVec(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Vector);
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 b = glm::vec4{ 2,2,3,4 };
				glm::vec4 c = glm::vec4{ 3,2,3,4 };

				glm::vec4 e = glm::vec4{ 5,2,3,4 };

				auto expression = [&datum, &e]() { datum.BackVec(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(c, datum.BackVec());

				auto expression2 = [&datum]() { datum.BackMat(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Matrix);
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 b = glm::mat4{ 2,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 c = glm::mat4{ 3,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				glm::mat4 e = glm::mat4{ 5,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				auto expression = [&datum, &e]() { datum.BackMat(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(c, datum.BackMat());

				auto expression2 = [&datum]() { datum.BackString(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::String);
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				std::string e = "5";

				auto expression = [&datum, &e]() { datum.BackString(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(c, datum.BackString());

				auto expression2 = [&datum]() { datum.BackPtr(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Pointer);
				Foo foo1 = 1;
				Foo foo2 = 2;
				Foo foo3 = 3;

				Foo foo5 = 5;
				RTTI* a = &foo1;
				RTTI* b = &foo2;
				RTTI* c = &foo3;

				RTTI* e = &foo5;

				auto expression = [&datum, &e]() { datum.BackPtr(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(c, datum.BackPtr());

				auto expression2 = [&datum]() { datum.BackInt(); };
				Assert::ExpectException<std::runtime_error>(expression2);

			}
		}

		TEST_METHOD(FindOverloads)
		{
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 5;
				int32_t b = 6;
				int32_t c = 7;
				int32_t e = 8;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(size_t(2), datum.Find(7).first);
				Assert::AreEqual(true, datum.Find(7).second);

				Assert::AreEqual(false, datum.Find(e).second);
				float test = 4;
				auto expression2 = [&datum, &test]() { datum.Find(test); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Float);
				float a = 1;
				float b = 2;
				float c = 3;

				float e = 5;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(size_t(2), datum.Find(c).first);
				Assert::AreEqual(true, datum.Find(c).second);

				Assert::AreEqual(false, datum.Find(e).second);

				glm::vec4 test = glm::vec4{ 1,2,3,4 };
				auto expression2 = [&datum, &test]() { datum.Find(test); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Vector);
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 b = glm::vec4{ 2,2,3,4 };
				glm::vec4 c = glm::vec4{ 3,2,3,4 };

				glm::vec4 e = glm::vec4{ 5,2,3,4 };

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(size_t(2), datum.Find(c).first);
				Assert::AreEqual(true, datum.Find(c).second);

				Assert::AreEqual(false, datum.Find(e).second);

				glm::mat4 test = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				auto expression2 = [&datum, &test]() { datum.Find(test); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Matrix);
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 b = glm::mat4{ 2,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 c = glm::mat4{ 3,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				glm::mat4 e = glm::mat4{ 5,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(size_t(2), datum.Find(c).first);
				Assert::AreEqual(true, datum.Find(c).second);

				Assert::AreEqual(false, datum.Find(e).second);

				std::string test = "3";
				auto expression2 = [&datum, &test]() { datum.Find(test); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::String);
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				std::string e = "5";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(size_t(2), datum.Find(c).first);
				Assert::AreEqual(true, datum.Find(c).second);

				Assert::AreEqual(false, datum.Find(e).second);

				Foo foo1 = 1;
				RTTI* test = &foo1;
				auto expression2 = [&datum, test]() { datum.Find(test); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Datum datum(DatumTypes::Pointer);
				Foo foo1 = 1;
				Foo foo2 = 2;
				Foo foo3 = 3;

				Foo foo5 = 5;
				RTTI* a = &foo1;
				RTTI* b = &foo2;
				RTTI* c = &foo3;

				RTTI* e = &foo5;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(size_t(2), datum.Find(c).first);
				Assert::AreEqual(true, datum.Find(c).second);

				Assert::AreEqual(false, datum.Find(e).second);

				int32_t test = 11;
				auto expression2 = [&datum, &test]() { datum.Find(test); };
				Assert::ExpectException<std::runtime_error>(expression2);

			}
		}

		TEST_METHOD(RemoveOverloads)
		{
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 5;
				int32_t b = 6;
				int32_t c = 7;
				int32_t e = 8;

				auto expression2 = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression2);
				
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(true, datum.Remove(b));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(c, datum.GetInt(1));

				Assert::AreEqual(false, datum.Remove(e));
			}

			{
				Datum datum(DatumTypes::Float);
				float a = 1;
				float b = 2;
				float c = 3;

				float e = 5;

				auto expression2 = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(true, datum.Remove(b));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(c, datum.GetFloat(1));

				Assert::AreEqual(false, datum.Remove(e));
			}

			{
				Datum datum(DatumTypes::Vector);
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 b = glm::vec4{ 2,2,3,4 };
				glm::vec4 c = glm::vec4{ 3,2,3,4 };

				glm::vec4 e = glm::vec4{ 5,2,3,4 };

				auto expression2 = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(true, datum.Remove(b));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(c, datum.GetVec(1));

				Assert::AreEqual(false, datum.Remove(e));
			}

			{
				Datum datum(DatumTypes::Matrix);
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 b = glm::mat4{ 2,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 c = glm::mat4{ 3,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				glm::mat4 e = glm::mat4{ 5,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				auto expression2 = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(true, datum.Remove(b));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(c, datum.GetMat(1));

				Assert::AreEqual(false, datum.Remove(e));
			}

			{
				Datum datum(DatumTypes::String);
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				std::string e = "5";

				auto expression2 = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(true, datum.Remove(b));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(c, datum.GetString(1));

				Assert::AreEqual(false, datum.Remove(e));
			}

			{
				Datum datum(DatumTypes::Pointer);
				Foo foo1 = 1;
				Foo foo2 = 2;
				Foo foo3 = 3;

				Foo foo5 = 5;
				RTTI* a = &foo1;
				RTTI* b = &foo2;
				RTTI* c = &foo3;

				RTTI* e = &foo5;

				auto expression2 = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(true, datum.Remove(b));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(c, datum.GetPtr(1));

				Assert::AreEqual(false, datum.Remove(e));

			}
		}

		TEST_METHOD(Resize)
		{
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 5;
				int32_t b = 6;
				int32_t c = 7;
				int32_t d = 8;
				int32_t e = 9;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				datum.PushBack(d);
				datum.PushBack(e);

				datum.Resize(2);
				
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(a, datum.FrontInt());
				Assert::AreEqual(b, datum.BackInt());
			}

			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 5;
				int32_t b = 6;
				int32_t c = 7;
				int32_t d = 8;
				int32_t e = 9;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				datum.PushBack(d);
				datum.PushBack(e);

				datum.Resize(10);

				Assert::AreEqual(size_t(10), datum.Size());
				Assert::AreEqual(size_t(10), datum.Capacity());
				Assert::AreEqual(a, datum.FrontInt());
			}

			{
				Datum datum(DatumTypes::String);
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";
				std::string d = "4";
				std::string e = "5";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				datum.PushBack(d);
				datum.PushBack(e);

				datum.Resize(2);

				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(a, datum.FrontString());
				Assert::AreEqual(b, datum.BackString());
			}

			{
				Datum datum(DatumTypes::String);
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";
				std::string d = "4";
				std::string e = "5";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				datum.PushBack(d);
				datum.PushBack(e);

				datum.Resize(10);

				Assert::AreEqual(size_t(10), datum.Size());
				Assert::AreEqual(size_t(10), datum.Capacity());
				Assert::AreEqual(a, datum.FrontString());
			}

			
		}

		TEST_METHOD(Equality)
		{		
			
			
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 5;
				int32_t b = 6;
				int32_t c = 7;
				int32_t d = 8;
				int32_t e = 9;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				datum.PushBack(d);
				datum.PushBack(e);

				Datum datum2(DatumTypes::Integer);
				int32_t a2 = 5;
				int32_t b2 = 6;
				int32_t c2 = 7;
				int32_t d2 = 8;
				int32_t e2 = 9;

				datum2.PushBack(a2);
				datum2.PushBack(b2);
				datum2.PushBack(c2);
				datum2.PushBack(d2);
				datum2.PushBack(e2);

				Datum datum3(DatumTypes::Integer);
				int32_t a3 = 5;
				int32_t b3 = 6;
				int32_t c3 = 7;
				int32_t d3 = 8;

				datum3.PushBack(a3);
				datum3.PushBack(b3);
				datum3.PushBack(c3);
				datum3.PushBack(d3);

				Datum datum4(DatumTypes::Integer);
				int32_t a4 = 15;
				int32_t b4 = 16;
				int32_t c4 = 17;
				int32_t d4 = 18;
				int32_t e4 = 19;

				datum4.PushBack(a4);
				datum4.PushBack(b4);
				datum4.PushBack(c4);
				datum4.PushBack(d4);
				datum4.PushBack(e4);

				Assert::AreEqual(true, datum == datum2);
				Assert::AreEqual(false, datum == datum3);
				Assert::AreEqual(false, datum == datum4);
			}
			{
				Datum datum(DatumTypes::String);
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";
				std::string d = "4";
				std::string e = "5";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				datum.PushBack(d);
				datum.PushBack(e);

				Datum datum2(DatumTypes::String);
				std::string a2 = "1";
				std::string b2 = "2";
				std::string c2 = "3";
				std::string d2 = "4";
				std::string e2 = "5";

				datum2.PushBack(a2);
				datum2.PushBack(b2);
				datum2.PushBack(c2);
				datum2.PushBack(d2);
				datum2.PushBack(e2);

				Datum datum3(DatumTypes::String);
				std::string a3 = "1";
				std::string b3 = "2";
				std::string c3 = "3";
				std::string d3 = "4";

				datum3.PushBack(a3);
				datum3.PushBack(b3);
				datum3.PushBack(c3);
				datum3.PushBack(d3);

				Datum datum4(DatumTypes::String);
				std::string a4 = "11";
				std::string b4 = "21";
				std::string c4 = "31";
				std::string d4 = "41";
				std::string e4 = "51";

				datum4.PushBack(a4);
				datum4.PushBack(b4);
				datum4.PushBack(c4);
				datum4.PushBack(d4);
				datum4.PushBack(e4);

				Assert::AreEqual(true, datum == datum2);
				Assert::AreEqual(false, datum != datum2);
				Assert::AreEqual(false, datum == datum3);
				Assert::AreEqual(false, datum == datum4);
			}
		}

		TEST_METHOD(EqualityOverloads)
		{
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 5;

				datum.PushBack(a);

				Assert::AreEqual(true, datum == 5);
				Assert::AreEqual(false, datum == 10);
				Assert::AreEqual(false, datum != 5);
				Assert::AreEqual(false, datum == "test");
			}
			{
				Datum datum(DatumTypes::Float);
				float a = 5;
				float b = 10;

				datum.PushBack(a);

				Assert::AreEqual(true, datum == a);
				Assert::AreEqual(false, datum == b);
				Assert::AreEqual(false, datum != a);
				Assert::AreEqual(false, datum == "test");
			}
			{
				Datum datum(DatumTypes::Vector);
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 b = glm::vec4{ 9,2,3,4 };

				datum.PushBack(a);

				Assert::AreEqual(true, datum == a);
				Assert::AreEqual(false, datum == b);
				Assert::AreEqual(false, datum != a);
				Assert::AreEqual(false, datum == "test");
			}
			{
				Datum datum(DatumTypes::Matrix);
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 b = glm::mat4{ 9,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };

				datum.PushBack(a);

				Assert::AreEqual(true, datum == a);
				Assert::AreEqual(false, datum == b);
				Assert::AreEqual(false, datum != a);
				Assert::AreEqual(false, datum == "test");
			}
			{
				Datum datum(DatumTypes::String);
				std::string a = "test1";
				std::string b = "hello";

				datum.PushBack(a);

				Assert::AreEqual(true, datum == a);
				Assert::AreEqual(false, datum == b);
				Assert::AreEqual(false, datum != a);
				Assert::AreEqual(false, datum == 5);
			}
			{
				Datum datum(DatumTypes::Pointer);
				Foo foo1 = 1;
				RTTI* a = &foo1;
				Foo foo2 = 2;
				RTTI* b = &foo2;

				datum.PushBack(a);

				Assert::AreEqual(true, datum == a);
				Assert::AreEqual(false, datum == b);
				Assert::AreEqual(false, datum != a);
				Assert::AreEqual(false, datum == "test");
			}

		}

		TEST_METHOD(ToString)
		{
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 5;

				auto expression = [&datum]() { datum.ToString(10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				std::string test = "5";
				Assert::AreEqual(test, datum.ToString(0));

			}
			{
				Datum datum(DatumTypes::Float);
				float a = 5;
				auto expression = [&datum]() { datum.ToString(10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				std::string test = "5.000000";
				Assert::AreEqual(test, datum.ToString(0));
			}
			{
				Datum datum(DatumTypes::Vector);
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				auto expression = [&datum]() { datum.ToString(10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				std::string test = "vec4(1.000000, 2.000000, 3.000000, 4.000000)";
				Assert::AreEqual(test, datum.ToString(0));
			}
			{
				Datum datum(DatumTypes::Matrix);
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				auto expression = [&datum]() { datum.ToString(10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				std::string test = "mat4x4((1.000000, 2.000000, 3.000000, 4.000000), (1.000000, 2.000000, 3.000000, 4.000000), (1.000000, 2.000000, 3.000000, 4.000000), (1.000000, 2.000000, 3.000000, 4.000000))";
				Assert::AreEqual(test, datum.ToString(0));
			}
			{
				Datum datum(DatumTypes::String);
				std::string a = "test1";

				auto expression = [&datum]() { datum.ToString(10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				std::string test = "test1";
				Assert::AreEqual(test, datum.ToString(0));
				
			}
			{
				Datum datum(DatumTypes::Pointer);
				Foo foo1 = 1;
				RTTI* a = &foo1;
				auto expression = [&datum]() { datum.ToString(10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				std::string test = "RTTI";
				Assert::AreEqual(test, datum.ToString(0));
			}

		}

		TEST_METHOD(SetFromString)
		{
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 5;
				std::string b = "10";
				int32_t c = 10;


				auto expression = [&datum, &b]() { datum.SetFromString(b, 10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				datum.SetFromString(b, 0);

				Assert::AreEqual(c, datum.FrontInt());
			}
			{
				Datum datum(DatumTypes::Float);
				float a = 5;
				std::string b = "10.000000";
				float c = 10;


				auto expression = [&datum, &b]() { datum.SetFromString(b, 10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				datum.SetFromString(b, 0);

				Assert::AreEqual(c, datum.FrontFloat());
			}
			{
				Datum datum(DatumTypes::Vector);
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				std::string b = "vec4(15.000000, 2.000000, 3.000000, 4.000000)";
				glm::vec4 c = glm::vec4{ 15,2,3,4 };


				auto expression = [&datum, &b]() { datum.SetFromString(b, 10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				datum.SetFromString(b, 0);

				Assert::AreEqual(c, datum.FrontVec());
			}
			{
				Datum datum(DatumTypes::Matrix);
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				std::string b = "mat4x4((15.000000, 2.000000, 3.000000, 4.000000), (1.000000, 2.000000, 3.000000, 4.000000), (1.000000, 2.000000, 3.000000, 4.000000), (1.000000, 2.000000, 3.000000, 4.000000))";
				glm::mat4 c = glm::mat4{ 15,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };


				auto expression = [&datum, &b]() { datum.SetFromString(b, 10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				datum.SetFromString(b, 0);

			}
			{
				Datum datum(DatumTypes::String);
				std::string a = "test1";

				std::string b = "test2";
				std::string c = "test2";
				


				auto expression = [&datum, &b]() { datum.SetFromString(b, 10); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);

				datum.SetFromString(b, 0);

				Assert::AreEqual(c, datum.FrontString());

			}
			{
				Datum datum(DatumTypes::Pointer);
				Foo foo1 = 1;
				RTTI* a = &foo1;
				std::string b = "test";

				datum.PushBack(a);

				auto expression2 = [&datum, &b]() { datum.SetFromString(b, 0); };
				Assert::ExpectException<std::runtime_error>(expression2);


			}

		}

		TEST_METHOD(SetStorage)
		{
			{
				Datum datum(DatumTypes::Integer);
				int32_t a = 5;
				int32_t testArray[2] = { 1,2 };
				int32_t* testArray2 = nullptr;
				datum.PushBack(a);

				auto expression = [&datum, &testArray]() { datum.SetStorage(testArray, 2); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &testArray2]() { datum.SetStorage(testArray2, 2); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);
				Assert::AreEqual(testArray[0], datum.FrontInt());
			}

			{
				Datum datum(DatumTypes::Float);
				float a = 5;
				float testArray[2] = { 1,2 };
				float* testArray2 = nullptr;
				datum.PushBack(a);

				auto expression = [&datum, &testArray]() { datum.SetStorage(testArray, 2); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &testArray2]() { datum.SetStorage(testArray2, 2); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
			{
				Datum datum;
				float testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);
				Assert::AreEqual(testArray[0], datum.FrontFloat());
			}

			{
				Datum datum(DatumTypes::Vector);
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 testArray[2] = { a,a };
				glm::vec4* testArray2 = nullptr;
				datum.PushBack(a);

				auto expression = [&datum, &testArray]() { datum.SetStorage(testArray, 2); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &testArray2]() { datum.SetStorage(testArray2, 2); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
			{
				Datum datum;
				glm::vec4 a = glm::vec4{ 1,2,3,4 };
				glm::vec4 testArray[2] = { a,a };

				datum.SetStorage(testArray, 2);
				Assert::AreEqual(testArray[0], datum.FrontVec());
			}

			{
				Datum datum(DatumTypes::Matrix);
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 testArray[2] = { a,a };
				glm::mat4* testArray2 = nullptr;
				datum.PushBack(a);

				auto expression = [&datum, &testArray]() { datum.SetStorage(testArray, 2); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &testArray2]() { datum.SetStorage(testArray2, 2); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
			{
				Datum datum;
				glm::mat4 a = glm::mat4{ 1,2,3,4, 1,2,3,4, 1, 2, 3, 4, 1, 2, 3, 4 };
				glm::mat4 testArray[2] = { a,a };

				datum.SetStorage(testArray, 2);
				Assert::AreEqual(testArray[0], datum.FrontMat());
			}

			{
				Datum datum(DatumTypes::String);
				std::string a = "tester";
				std::string testArray[2] = { a,a };
				std::string* testArray2 = nullptr;
				datum.PushBack(a);

				auto expression = [&datum, &testArray]() { datum.SetStorage(testArray, 2); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &testArray2]() { datum.SetStorage(testArray2, 2); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
			{
				Datum datum;
				std::string a = "tester";
				std::string testArray[2] = { a,a };

				datum.SetStorage(testArray, 2);
				Assert::AreEqual(testArray[0], datum.FrontString());
			}

			/* {
				Datum datum(DatumTypes::Pointer);
				Foo foo1 = 1;
				RTTI* a = &foo1;
				RTTI** testArray;
				testArray[0] = a;
				testArray[1] = a;
				RTTI** testArray2 = nullptr;
				datum.PushBack(a);

				auto expression = [&datum, testArray]() { datum.SetStorage(testArray, 2); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, testArray2]() { datum.SetStorage(testArray2, 2); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
			{
				Datum datum(DatumTypes::Pointer);
				Foo foo1 = 1;
				RTTI* a = &foo1;
				RTTI** testArray;
				testArray[0] = a;
				testArray[1] = a;

				datum.SetStorage(testArray, 2);
				Assert::AreEqual(testArray[0], datum.FrontPtr());
			}*/
			

		}

		TEST_METHOD(ExternalGuards)
		{
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);
				
				auto expression = [&datum]() { datum.PushBack(20); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &testArray]() { datum.SetStorage(testArray, 2); };
				Assert::ExpectException<std::runtime_error>(expression);
			}
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				auto expression = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				auto expression = [&datum]() { datum.Clear(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				auto expression = [&datum]() { datum.RemoveAt(0); };
				Assert::ExpectException<std::runtime_error>(expression);
			}
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				auto expression = [&datum]() { datum.Remove(1); };
				Assert::ExpectException<std::runtime_error>(expression);
			}
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				auto expression = [&datum]() { datum = 1; };
				Assert::ExpectException<std::runtime_error>(expression);
			}
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				auto expression = [&datum]() { datum.ShrinkToFit(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				auto expression = [&datum]() { datum.Resize(40); };
				Assert::ExpectException<std::runtime_error>(expression);
			}
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				auto expression = [&datum]() { datum.Reserve(20); };
				Assert::ExpectException<std::runtime_error>(expression);
			}
		}

		TEST_METHOD(Copy)
		{
			{
				Datum datum;
				int32_t a = 1;
				int32_t b = 2;
				int32_t c = 3;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Datum datum2 = datum;

				Assert::AreEqual(a, datum2.FrontInt());
				Assert::AreEqual(datum.Size(), datum2.Size());
				Assert::AreEqual(datum.Capacity(), datum2.Capacity());
				Assert::AreEqual(datum.Type(), datum2.Type());
			}

			{
				Datum datum;
				int32_t a = 1;
				int32_t b = 2;
				int32_t c = 3;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Datum datum2;
				int32_t a2 = 4;
				int32_t b2 = 5;
				int32_t c2 = 6;
				int32_t d2 = 7;

				datum2.PushBack(a2);
				datum2.PushBack(b2);
				datum2.PushBack(c2);
				datum2.PushBack(d2);

				datum2 = datum;

				Assert::AreEqual(a, datum2.FrontInt());
				Assert::AreEqual(datum.Size(), datum2.Size());
				Assert::AreEqual(datum.Capacity(), datum2.Capacity());
				Assert::AreEqual(datum.Type(), datum2.Type());
			}

			{
				Datum datum;
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Datum datum2 = datum;

				Assert::AreEqual(a, datum2.FrontString());
				Assert::AreEqual(datum.Size(), datum2.Size());
				Assert::AreEqual(datum.Capacity(), datum2.Capacity());
				Assert::AreEqual(datum.Type(), datum2.Type());
			}

			{
				Datum datum;
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Datum datum2;
				std::string a2 = "4";
				std::string b2 = "5";
				std::string c2 = "6";
				std::string d2 = "7";

				datum2.PushBack(a2);
				datum2.PushBack(b2);
				datum2.PushBack(c2);
				datum2.PushBack(d2);

				datum2 = datum;

				Assert::AreEqual(a, datum2.FrontString());
				Assert::AreEqual(datum.Size(), datum2.Size());
				Assert::AreEqual(datum.Capacity(), datum2.Capacity());
				Assert::AreEqual(datum.Type(), datum2.Type());
			}

			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				Datum datum2 = datum;
			}
			
			{
				Datum datum;
				int32_t testArray[2] = { 1,2 };

				datum.SetStorage(testArray, 2);

				Datum datum2;
				int32_t a2 = 4;
				int32_t b2 = 5;
				int32_t c2 = 6;
				int32_t d2 = 7;

				datum2.PushBack(a2);
				datum2.PushBack(b2);
				datum2.PushBack(c2);
				datum2.PushBack(d2);

				datum2 = datum;
			}
			
		}

		TEST_METHOD(Move)
		{
			{
				Datum datum;
				int32_t a = 1;
				int32_t b = 2;
				int32_t c = 3;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Datum datum2 = std::move(datum);

				Assert::AreEqual(a, datum2.FrontInt());
				Assert::AreEqual(size_t(3), datum2.Size());
				Assert::AreEqual(size_t(6), datum2.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum2.Type());
			}

			{
				Datum datum;
				int32_t a = 1;
				int32_t b = 2;
				int32_t c = 3;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Datum datum2;
				int32_t a2 = 4;
				int32_t b2 = 5;
				int32_t c2 = 6;
				int32_t d2 = 7;

				datum2.PushBack(a2);
				datum2.PushBack(b2);
				datum2.PushBack(c2);
				datum2.PushBack(d2);

				datum2 = std::move(datum);

				Assert::AreEqual(a, datum2.FrontInt());
				Assert::AreEqual(size_t(3), datum2.Size());
				Assert::AreEqual(size_t(6), datum2.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum2.Type());
			}

			{
				Datum datum;
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Datum datum2 = std::move(datum);

				Assert::AreEqual(a, datum2.FrontString());
				Assert::AreEqual(size_t(3), datum2.Size());
				Assert::AreEqual(size_t(6), datum2.Capacity());
				Assert::AreEqual(DatumTypes::String, datum2.Type());
			}

			{
				Datum datum;
				std::string a = "1";
				std::string b = "2";
				std::string c = "3";

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Datum datum2;
				std::string a2 = "4";
				std::string b2 = "5";
				std::string c2 = "6";
				std::string d2 = "7";

				datum2.PushBack(a2);
				datum2.PushBack(b2);
				datum2.PushBack(c2);
				datum2.PushBack(d2);

				datum2 = std::move(datum);

				Assert::AreEqual(a, datum2.FrontString());
				Assert::AreEqual(size_t(3), datum2.Size());
				Assert::AreEqual(size_t(6), datum2.Capacity());
				Assert::AreEqual(DatumTypes::String, datum2.Type());
			}

		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState DatumTests::_startMemState;
}