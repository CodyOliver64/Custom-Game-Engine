#include "pch.h"
#include "WorldManager.h"
#include "GameObject.h"

namespace FIEAGameEngine
{
	WorldManager::WorldManager(GameObject* rootObject) :
		_rootObject(rootObject)
	{
	}

	void WorldManager::Update()
	{
		assert(_rootObject != nullptr);

		if (_gameClockEnabled)
		{
			_clock.UpdateGameTime(_time);
		}

		_rootObject->Update(*this);
		_eventQueue.Update(_time);
		updateActions();
	}
	
	void WorldManager::updateActions()
	{
		for (size_t i = 0; i < _pendingActionsCreated.Size(); ++i)
		{
			auto [child, parent] = _pendingActionsCreated[i]; //Make sure this will build with c++17
			parent->Adopt(*child, "Actions");
		}

		for (size_t i = 0; i < _pendingActionsRemoved.Size(); ++i)
		{
			delete _pendingActionsRemoved[i];
		}

		_pendingActionsCreated.Clear();
		_pendingActionsRemoved.Clear();
	}
}
