#pragma once
#include "Vector.h"
#include "Scope.h"
#include "GameClock.h"
#include "EventQueue.h"

namespace FIEAGameEngine
{
	/// <summary>
	/// WorldManager is a class that hold runtime game data that is used during the main Update Loop
	/// The class will maintain a root gameObject and manage the Update loop along with the game time
	/// </summary>
	class GameObject;
	class WorldManager final
	{
		
	public:
		WorldManager() = default;
		~WorldManager() = default;
		WorldManager(const WorldManager&) = default;
		WorldManager& operator=(const WorldManager&) = default;
		WorldManager(WorldManager&&) noexcept = default;
		WorldManager& operator=(WorldManager&&) noexcept = default;

		/// <summary>
		/// Constructor that takes in the root game object to be used by the manager instance
		/// </summary>
		/// <param name="rootObject">root GameObject</param>
		WorldManager(GameObject* rootObject);
		
		
		/// <summary>
		/// Main Update loop, will begin the chain update on the root game object and then update events and actions based on the pending queue that was built during the update loop
		/// </summary>
		void Update();
		
	
		/// <summary>
		/// updateActions will iterate through the lists of pending actions to be added/removed. These lists are created during the update frame and can be added to the hierarchy at any after the initial call to Update
		/// This is to deffer adding/removing new actions during an update loop
		/// </summary>
		void updateActions();

		
		GameObject* _rootObject{ nullptr };
		GameClock _clock;
		GameTime _time;
		EventQueue _eventQueue;
		Vector<std::pair<Scope*, Scope*>> _pendingActionsCreated;
		Vector<Scope*> _pendingActionsRemoved;
		bool _gameClockEnabled{ false };
	};
}

