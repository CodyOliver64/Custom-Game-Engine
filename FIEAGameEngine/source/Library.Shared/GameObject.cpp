#include "pch.h"
#include "GameObject.h"
#include <assert.h>

namespace FIEAGameEngine
{
	RTTI_DEFINITIONS(GameObject)

	GameObject::GameObject() :
		Attributed(GameObject::TypeIdClass())
	{
		transform = new Transform();
	}

	GameObject::GameObject(size_t typeID) :
		Attributed(typeID)
	{
	}

	GameObject::~GameObject()
	{
		delete transform;
	}

	GameObject::GameObject(const GameObject& other) :
		Attributed(other), objName(other.objName)
	{
		transform = new Transform();
	}

	GameObject& GameObject::operator=(const GameObject& other)
	{
		if (this != &other)
		{
			Attributed::Attributed(other);
			objName = other.objName;
			delete transform;
			transform = new Transform(*other.transform);
		}

		return *this;
	}

	GameObject::GameObject(GameObject&& other) noexcept :
		Attributed(std::move(other)), objName(std::move(other.objName)), transform(other.transform)
	{
		other.transform = nullptr;
	}

	GameObject& GameObject::operator=(GameObject&& other) noexcept
	{
		if (this != &other)
		{
			Attributed::Attributed(std::move(other));
			objName = std::move(other.objName);
			delete transform;
			transform = other.transform;
			other.transform = nullptr;
		}

		return *this;
	}

	
	void GameObject::Update(WorldManager& worldState)
	{
		//Update Children
		for (size_t i = 0; i < _children.Size(); ++i)
		{
			assert(_children.GetScope(i)->Is(GameObject::TypeIdClass()));
			_children.GetScope(i)->As<GameObject>()->Update(worldState);
		}

		//Update Actions
		for (size_t i = 0; i < _actions.Size(); ++i)
		{
			assert(_actions.GetScope(i)->Is(Action::TypeIdClass()));
			_actions.GetScope(i)->As<Action>()->Update(worldState);
		}
	}

	const Datum& GameObject::GetActions() const
	{
		return _actions;
	}
	const Datum& GameObject::GetChildren() const
	{
		return _children;
	}
	void GameObject::CreateAction(const std::string className, const std::string instanceName)
	{
		Action* newAction = Factory<Scope>::Create(className)->As<Action>();
		assert(newAction->Is(Action::TypeIdClass()));
		newAction->SetName(instanceName);
		Adopt(*newAction, "Actions");
	}
	
	const Vector<Signature> GameObject::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", DatumTypes::String, 1, offsetof(GameObject, objName) },
			{ "Transform", DatumTypes::Table, 1, offsetof(GameObject, transform) },
			{ "Children", DatumTypes::Table, 0,  0 },
			{ "Actions", DatumTypes::Table, 0,  0 }
		};
	}

	gsl::owner<GameObject*> GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	std::string GameObject::ToString() const
	{
		return "GameObject";
	}

	bool GameObject::Equals(const RTTI* rhs) const
	{
		const auto rhsAttFoo = rhs->As<GameObject>();

		if (rhsAttFoo == nullptr)
		{
			return false;
		}

		return objName == rhsAttFoo->objName;
	}
}
