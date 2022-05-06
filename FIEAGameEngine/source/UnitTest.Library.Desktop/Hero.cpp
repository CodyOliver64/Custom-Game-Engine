#include "pch.h"
#include "Hero.h"



namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(Hero)

	Hero::Hero() :
		GameObject(Hero::TypeIdClass())
	{
	}

	Hero::Hero(size_t typeID) :
		GameObject(typeID)
	{
	}


	void Hero::Update(WorldManager& worldState)
	{
		GameObject::Update(worldState); //monster gets updated first, or monsters children first
	}


	const Vector<Signature> Hero::Signatures()
	{
		return Vector<Signature>
		{
			{ "Health", DatumTypes::Integer, 1, offsetof(Hero, Health) },
		};
	}

	gsl::owner<Hero*> Hero::Clone() const
	{
		return new Hero(*this);
	}

	std::string Hero::ToString() const
	{
		return "Hero";
	}

	bool Hero::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<Hero>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return Health == rhsAttFoo->Health;
	}
}
