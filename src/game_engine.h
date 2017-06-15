#ifndef RETRO3D_GAMEENGINE_H
#define RETRO3D_GAMEENGINE_H

/*=============================================================================================
Game Engine

The core of the game egine.
Contains:
	- All managers (InputManager, ScriptManager)
	- World (where objects exist)
	and more.
The game is ticked from here.
==============================================================================================*/

#include "objectptr.h"
#include "config_reader.h"

namespace Retro3D
{
	class Level;
	class Window;
	class InputManager;
	class ScriptManager;
	class SceneRenderer;
	class World;
	class PlayerController;
	class WorldMessageBus;

	class GameEngine
	{
	private:
		Level* mCurrentLevel;
		Window* mWindow;
		InputManager* mInputManager;
		ScriptManager* mScriptManager;
		SceneRenderer* mSceneRenderer;
		World* mWorld;
		PlayerController* mPlayerController;
		float mDeltaTime = 0.1f;
		bool mIsRunning;
		ConfigReader mGameConfig;
		WorldMessageBus* mWorldMessageBus;

		GameEngine();

	public:
		static GameEngine* CreateGameEngine();

	public:
		void StartEngine();
		void TickEngine();

		float GetDeltaTime();

		void SetPlayerController(PlayerController* arg_controller) { mPlayerController = arg_controller; } // TODO

	public:
		inline Level* GetCurrentLevel() { return mCurrentLevel; }
		inline Window* GetWindow() { return mWindow; }
		inline InputManager* GetInputManager() { return mInputManager; }
		inline ScriptManager* GetScriptManager() { return mScriptManager; }
		inline SceneRenderer* GetSceneRenderer() { return mSceneRenderer; }
		inline World* GetWorld() { return mWorld; }
		inline PlayerController* GetPlayerController() { return mPlayerController; }
		inline ConfigReader& GetGameConfig() { return mGameConfig; }
		inline WorldMessageBus* GetWorldMessageBus() { return mWorldMessageBus; }
	};
}

namespace Retro3D
{
	extern GameEngine* GGameEngine;
}

#endif
