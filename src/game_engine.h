#ifndef RETRO3D_GAMEENGINE_H
#define RETRO3D_GAMEENGINE_H

#include "objectptr.h"

namespace Retro3D
{
	class Level;
	class Window;
	class InputManager;
	class SceneRenderer;
	class World;
	class PlayerController;

	class GameEngine
	{
	private:
		Level* mCurrentLevel;
		Window* mWindow;
		InputManager* mInputManager;
		SceneRenderer* mSceneRenderer;
		World* mWorld;
		PlayerController* mPlayerController;
		float mDeltaTime = 0.1f;
		bool mIsRunning;

		GameEngine();

	public:
		static GameEngine* CreateGameEngine();

	public:
		void StartEngine();
		void TickEngine();

		void SetPlayerController(PlayerController* arg_controller) { mPlayerController = arg_controller; } // TODO

	public:
		inline Level* GetCurrentLevel() { return mCurrentLevel; }
		inline Window* GetWindow() { return mWindow; }
		inline InputManager* GetInputManager() { return mInputManager; }
		inline SceneRenderer* GetSceneRenderer() { return mSceneRenderer; }
		inline World* GetWorld() { return mWorld; }
		inline PlayerController* GetPlayerController() { return mPlayerController; }
	};
}

namespace Retro3D
{
	extern GameEngine* GGameEngine;
}

#endif
