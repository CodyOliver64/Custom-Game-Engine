#include "pch.h"
#include "Monster.h"



namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Monster)

	Monster::Monster() :
		GameObject(Monster::TypeIdClass())
	{
	}

	Monster::Monster(size_t typeID) :
		GameObject(typeID)
	{
	}


	void Monster::Update(WorldManager& worldState)
	{
		GameObject::Update(worldState); //monster gets updated first, or monsters children first

		--Health;
	}


	const Vector<Signature> Monster::Signatures()
	{
		return Vector<Signature>
		{
			{ "Health", DatumTypes::Integer, 1, offsetof(Monster, Health) },
		};
	}

	gsl::owner<Monster*> Monster::Clone() const
	{
		return new Monster(*this);
	}

	std::string Monster::ToString() const
	{
		return "Monster";
	}

	bool Monster::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<Monster>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return Health == rhsAttFoo->Health;
	}
}
