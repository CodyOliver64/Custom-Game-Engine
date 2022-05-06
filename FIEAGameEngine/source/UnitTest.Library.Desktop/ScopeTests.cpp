#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "HashMap.h"
#include "Scope.h"
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
	TEST_CLASS(ScopeTests)
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


		TEST_METHOD(Append_Find)
		{
			Scope scope;

			Datum& e1Datum = scope.Append("entry1");
			Datum& e2Datum = scope.Append("entry2");

			Assert::AreEqual(size_t(2), scope.vectorSize());
			Assert::AreEqual(size_t(2), scope.hashMapSize());

			e1Datum.PushBack(1);
			e2Datum.PushBack(2);

			Datum* foundE1Datum = scope.Find("entry1");
			Datum* foundE2Datum = scope.Find("entry2");

			Assert::AreEqual(1, foundE1Datum->FrontInt());
			Assert::AreEqual(2, foundE2Datum->FrontInt());

			Datum* notFoundDatum = scope.Find("scopeyScoper");

			Assert::AreEqual(true, notFoundDatum == nullptr);
		}

		TEST_METHOD(AppendScope)
		{
			Scope scope;

			Datum& e1Datum = scope.Append("entry1");
			Datum& e2Datum = scope.Append("entry2");

			e1Datum.PushBack(1);
			e2Datum.PushBack(2);

			Scope& e1Scope = scope.AppendScope("scopeContainer1");


			Datum* foundScopeDatum = scope.Find("scopeContainer1");
			Assert::AreEqual(true, foundScopeDatum->Type() == DatumTypes::Table);

			Scope* e1ScopePtr = foundScopeDatum->GetScope(0);

			Scope& e2Scope = scope.AppendScope("scopeContainer1");

			Scope* e2ScopePtr = foundScopeDatum->GetScope(1);

			Assert::AreEqual(size_t(2), foundScopeDatum->Size());

			Assert::AreEqual(e1ScopePtr, &e1Scope);

			Assert::AreEqual(e2ScopePtr, &e2Scope);

			Assert::AreEqual(e2ScopePtr->GetParent(), e1Scope.GetParent());

			auto expression = [&scope]() { scope.AppendScope("entry1"); };
			Assert::ExpectException<std::runtime_error>(expression);
		}

		TEST_METHOD(Search)
		{
			Scope scope;

			Scope& e1Scope = scope.AppendScope("scopeContainer1");

			Datum& e1Datum = scope.Append("entry1");
			Datum& e2Datum = scope.Append("entry2");

			e1Datum.PushBack(1);
			e2Datum.PushBack(2);

			Datum* localSearchedDatum = scope.Search("entry1");
			Datum* childSearchedDatum = e1Scope.Search("entry2");

			Assert::AreEqual(localSearchedDatum, &e1Datum);
			Assert::AreEqual(childSearchedDatum, &e2Datum);

			Datum* notFoundDatum = scope.Search("scopeyScoper");

			Assert::AreEqual(true, notFoundDatum == nullptr);

			Scope* outputScope1 = nullptr;
			Scope* outputScope2 = nullptr;
			Scope* outputScope3 = nullptr;


			Datum* localSearchedDatum2 = scope.Search("entry1", outputScope1);
			Datum* childSearchedDatum2 = e1Scope.Search("entry2", outputScope2);

			Assert::AreEqual(localSearchedDatum2, &e1Datum);
			Assert::AreEqual(childSearchedDatum2, &e2Datum);

			Assert::AreEqual(outputScope1, outputScope2);

			Datum* notFoundDatum2 = scope.Search("scopeyScoper", outputScope3);
			Assert::AreEqual(true, notFoundDatum2 == nullptr);
		}

		TEST_METHOD(ConstSearch)
		{
			{
				Scope scope;

				Scope& e1Scope = scope.AppendScope("scopeContainer1");

				scope.Append("entry1");
				scope.Append("entry2");

				e1Scope.AppendScope("nestSearch");

				const Scope& scope2 = scope;

				scope2.Search("entry1");
				scope2.Search("nestSearch");
				e1Scope.Search("entry2");
			}

			{
				Scope scope;

				Scope& e1Scope = scope.AppendScope("scopeContainer1");

				scope.Append("entry1");
				scope.Append("entry2");

				const Scope& newscope = e1Scope.AppendScope("nestSearch");

				//const Scope& scope2 = e1Scope;

				
				newscope.Search("entry1");
			}

			{
				Scope scope;
				const Scope* contScopePtr;

				Scope& e1Scope = scope.AppendScope("scopeContainer1");

				scope.Append("entry1");
				scope.Append("entry2");

				const Scope& newscope = e1Scope.AppendScope("nestSearch");

				//const Scope& scope2 = e1Scope;


				newscope.Search("entry1", contScopePtr);
				Assert::IsNotNull(contScopePtr);
			}

		}

		TEST_METHOD(FindContainedScope)
		{
			{
				Scope scope;
				scope.AppendScope("scopeContainer1");
				Datum& e1Datum = scope.Append("entry1");
				Datum& e2Datum = scope.Append("entry2");
				e1Datum.PushBack(1);
				e2Datum.PushBack(2);

				Scope* scope2 = new Scope();
				scope2->AppendScope("other_scopeContainer1");
				Datum& other_e1Datum = scope2->Append("other_entry1");
				Datum& other_e2Datum = scope2->Append("other_entry2");
				other_e1Datum.PushBack(3);
				other_e2Datum.PushBack(4);

				auto result2 = scope.FindContainedScope(scope2);
				Assert::AreEqual(true, result2.first == nullptr);

				scope.Adopt(*scope2, "adoptedContainer");

				auto result = scope.FindContainedScope(scope2);

				Assert::AreEqual(result.first, scope.Find("adoptedContainer"));
				Assert::AreEqual(size_t(0), result.second);

			}
		}
		
		TEST_METHOD(Adopt)
		{
			//adopt a scope that has no parent (while creating a new scope datum)
			{
				Scope scope;
				scope.AppendScope("scopeContainer1");
				Datum& e1Datum = scope.Append("entry1");
				Datum& e2Datum = scope.Append("entry2");
				e1Datum.PushBack(1);
				e2Datum.PushBack(2);

				Scope* scope2 = new Scope();
				scope2->AppendScope("other_scopeContainer1");
				Datum& other_e1Datum = scope2->Append("other_entry1");
				Datum& other_e2Datum = scope2->Append("other_entry2");
				other_e1Datum.PushBack(3);
				other_e2Datum.PushBack(4);

				Assert::ExpectException<std::runtime_error>([&scope, &scope2] {scope.Adopt(*scope2, "entry1"); });

				scope.Adopt(*scope2, "adoptedContainer");

				Assert::AreEqual(&scope, scope2->GetParent());
				Datum* foundNewScopeTable = scope.Find("adoptedContainer");
				Assert::AreEqual(true, foundNewScopeTable != nullptr);

				Scope* outputScope1 = nullptr;
				scope2->Search("entry1", outputScope1);
				Assert::AreEqual(&scope, outputScope1);

				Assert::AreEqual(&scope, outputScope1);

				auto result = scope.FindContainedScope(scope2);
				Assert::AreEqual(result.first, foundNewScopeTable);
			}

			//adopt a scope that has no parent (without creating a new scope datum)
			{
				Scope scope;
				scope.AppendScope("scopeContainer1");
				Datum& e1Datum = scope.Append("entry1");
				Datum& e2Datum = scope.Append("entry2");
				e1Datum.PushBack(1);
				e2Datum.PushBack(2);

				Scope* scope2 = new Scope();
				scope2->AppendScope("other_scopeContainer1");
				Datum& other_e1Datum = scope2->Append("other_entry1");
				Datum& other_e2Datum = scope2->Append("other_entry2");
				other_e1Datum.PushBack(3);
				other_e2Datum.PushBack(4);

				scope.Adopt(*scope2, "adoptedContainer");

				Assert::AreEqual(&scope, scope2->GetParent());
				Datum* foundNewScopeTable = scope.Find("adoptedContainer");
				Assert::AreEqual(true, foundNewScopeTable != nullptr);

				Scope* outputScope1 = nullptr;
				scope2->Search("entry1", outputScope1);
				Assert::AreEqual(&scope, outputScope1);

				Assert::AreEqual(&scope, outputScope1);

				auto result = scope.FindContainedScope(scope2);
				Assert::AreEqual(result.first, foundNewScopeTable);

				Scope* scope3 = new Scope();
				scope3->AppendScope("other_scopeContainer3");
				Datum& other_e1Datum3 = scope3->Append("other_entry13");
				Datum& other_e2Datum3 = scope3->Append("other_entry23");
				other_e1Datum3.PushBack(3);
				other_e2Datum3.PushBack(4);

				scope.Adopt(*scope3, "adoptedContainer");

				Assert::AreEqual(&scope, scope3->GetParent());
				Datum* foundNewScopeTable3 = scope.Find("adoptedContainer");
				Assert::AreEqual(true, foundNewScopeTable3 != nullptr);
			}

			//adopt a scope that has no parent (without creating a new scope datum)
			{
				Scope scope;
				scope.AppendScope("scopeContainer1");
				Datum& e1Datum = scope.Append("entry1");
				Datum& e2Datum = scope.Append("entry2");
				e1Datum.PushBack(1);
				e2Datum.PushBack(2);

				Scope* scope2 = new Scope();
				scope2->AppendScope("other_scopeContainer1");
				Datum& other_e1Datum = scope2->Append("other_entry1");
				Datum& other_e2Datum = scope2->Append("other_entry2");
				other_e1Datum.PushBack(3);
				other_e2Datum.PushBack(4);

				scope.Adopt(*scope2, "adoptedContainer");

				Scope anotherScope;
				anotherScope.Adopt(*scope2, "anotherContainer");

				Assert::AreEqual(&anotherScope, scope2->GetParent());
				Datum* foundNewScopeTable = anotherScope.Find("anotherContainer");
				Assert::AreEqual(true, foundNewScopeTable != nullptr);

				auto result = anotherScope.FindContainedScope(scope2);
				Assert::AreEqual(result.first, foundNewScopeTable);
			}

			{
				Scope* scope = new Scope();
				scope->AppendScope("scopeContainer1");
				Datum& e1Datum = scope->Append("entry1");
				Datum& e2Datum = scope->Append("entry2");
				e1Datum.PushBack(1);
				e2Datum.PushBack(2);

				Scope* scope2 = new Scope();
				scope2->AppendScope("other_scopeContainer1");
				Datum& other_e1Datum = scope2->Append("other_entry1");
				Datum& other_e2Datum = scope2->Append("other_entry2");
				other_e1Datum.PushBack(3);
				other_e2Datum.PushBack(4);

				Scope& sdkjhfsdkjhf = scope2->AppendScope("newScope");

				scope->Adopt(*scope2, "adoptContainer");

				Assert::ExpectException<std::runtime_error>([&sdkjhfsdkjhf, &scope] {sdkjhfsdkjhf.Adopt(*scope, "entry1"); });

				delete scope;
			}
		}

		TEST_METHOD(Brackets)
		{
			Scope scope;
			scope.AppendScope("scopeContainer1");
			Datum& e1Datum = scope.Append("entry1");
			Datum& e2Datum = scope.Append("entry2");
			e1Datum.PushBack(1);
			e2Datum.PushBack(2);

			auto indexDatum = scope[1];
			Assert::AreEqual(e1Datum, indexDatum);
			
			auto newDatum = scope["entry1"];

			Assert::AreEqual(e1Datum, newDatum);
			Assert::AreEqual(e1Datum, newDatum);

			auto newDatum2 = scope["entry7"];
			auto result = scope.Find("entry7");

			Assert::AreEqual(*result, newDatum2);
		}

		TEST_METHOD(MoveConstructor)
		{
			Scope scope;

			Datum& aDatum = scope.Append("entry1");
			aDatum.PushBack(1);
			Datum& bDatum = scope.Append("entry2");
			bDatum.PushBack(2);

			Scope& cScope = scope.AppendScope("entry3");
			cScope.AppendScope("entry4");

			Scope movedScope;
			movedScope = std::move(scope);
			Assert::AreEqual(movedScope.hashMapSize(), size_t(3));
			Datum* foundDatum = movedScope.Find("entry1");
			Assert::AreEqual(&aDatum, foundDatum);

			Datum* foundScopeDatum = movedScope.Find("entry3");
			Assert::IsNotNull(foundScopeDatum->GetScope(0));
			Assert::AreEqual(foundScopeDatum->GetScope(0)->GetParent(), &movedScope);

			Scope* reparentedScope = new Scope(std::move(cScope));
			Assert::AreEqual(&movedScope, reparentedScope->GetParent());
			Datum* foundDatum2 = movedScope.Find("entry3");
			Assert::IsNotNull(foundDatum2);

			delete& cScope;

		}
		
		TEST_METHOD(MoveAssignment)
		{
			Scope scope;
			
			Datum& aDatum = scope.Append("entry1");
			aDatum.PushBack(1);
			Datum& bDatum = scope.Append("entry2");
			bDatum.PushBack(2);

			Scope& cScope = scope.AppendScope("entry3");
			cScope.AppendScope("entry4");

			Scope movedScope;
			movedScope = std::move(scope);
			Assert::AreEqual(movedScope.hashMapSize(), size_t(3));
			Datum* foundDatum = movedScope.Find("entry1");
			Assert::AreEqual(&aDatum, foundDatum);

			Datum* foundScopeDatum = movedScope.Find("entry3");
			Assert::IsNotNull(foundScopeDatum->GetScope(0));
			Assert::AreEqual(foundScopeDatum->GetScope(0)->GetParent(), &movedScope);

			Scope* reparentedScope = new Scope();
			*reparentedScope = std::move(cScope);
			Assert::AreEqual(&movedScope, reparentedScope->GetParent());
			Datum* foundDatum2 = movedScope.Find("entry3");
			Assert::IsNotNull(foundDatum2);

			delete& cScope;
		}

		TEST_METHOD(CopyAssignment)
		{
			Scope scope1;
			Scope scope2;

			scope2.Append("entry1");
			scope2.AppendScope("entry2");

			scope1.Append("entry3");
			scope1.AppendScope("entry4");

			Assert::IsNotNull(scope2.Find("entry1"));
			Assert::IsNull(scope2.Find("entry3"));

			scope2 = scope1;

			Assert::IsNull(scope2.Find("entry1"));
			Assert::IsNotNull(scope2.Find("entry3"));
		}
		TEST_METHOD(CopyConstructor)
		{
			Scope scope2;

			scope2.Append("entry1");
			scope2.AppendScope("entry2");

			Scope scope1 = scope2;

			Assert::IsNotNull(scope1.Find("entry1"));
			Assert::IsNull(scope1.Find("entry3"));

			Assert::AreEqual(scope1.Find("entry2")->GetScope()->GetParent(), &scope1);
		}

		TEST_METHOD(Equality)
		{
			{
				Scope scope1;
				Scope scope2;

				scope1.Append("entry1");
				Datum* datum1 = scope1.Find("entry1");
				datum1->PushBack(10);

				scope2.Append("entry1");
				Datum* datum2 = scope2.Find("entry1");
				datum2->PushBack(10);

				scope1.AppendScope("scopeTable");
				scope2.AppendScope("scopeTable");

				Assert::IsTrue(scope1 == scope2);
				datum1->PushBack(20);
				Assert::IsTrue(scope1 != scope2);
				Assert::IsTrue(scope1 == scope1);

				scope1.Append("entry7");
				Assert::IsTrue(scope1 != scope2);
				scope2.Append("entry8");
				Assert::IsTrue(scope1 != scope2);
			}

			{
				Scope scope1;
				Scope scope2;

				scope1.Append("entry1");
				Datum* datum1 = scope1.Find("entry1");
				datum1->PushBack(10);

				scope2.Append("entry2");
				Datum* datum2 = scope2.Find("entry2");
				datum2->PushBack(10);

				scope1.AppendScope("scopeTable");
				scope2.AppendScope("scopeTable");

				Assert::IsTrue(scope1 != scope2);
			}

			{
				Scope scope1;
				Scope scope2;

				scope1.Append("entry1");
				Datum* datum1 = scope1.Find("entry1");
				datum1->PushBack(10);

				scope2.Append("entry2");
				Datum* datum2 = scope2.Find("entry2");
				datum2->PushBack(10);

				scope1.AppendScope("scopeTable");
				scope2.AppendScope("scopeTable");

				RTTI* rtti{ nullptr };
				Assert::IsFalse(scope1.Equals(rtti));
			}
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ScopeTests::_startMemState;
}