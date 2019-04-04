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

#include "Object/objectptr.h"
#include "Text/config_reader.h"
#include "Graphics/render_target.h"
#include "Window/window.h"
#include <functional>

namespace Retro3D
{
	// Forward declarations
	class Level;
	class Window;
	class InputManager;
	class ScriptManager;
	class WidgetManager;
	class SceneRenderer;
	class ResourceManager;
	class World;
	class PlayerController;
	class WorldMessageBus;
	class IWidgetRenderer;
	class AudioManager;
	class InputMethodManager;

	class GameEngine
	{
	private:
		World* mWorld;
		WorldMessageBus* mWorldMessageBus;
		Level* mCurrentLevel;
		IRenderTarget* mRenderTarget; // TODO: support multiple render targets
		WindowBase* mMainWindow;
		WindowBase* mFocusedWindow;

		// Managers
		InputManager* mInputManager;
		ScriptManager* mScriptManager;
		WidgetManager* mWidgetManager;
		SceneRenderer* mSceneRenderer;
		ResourceManager* mResourceManager;
		AudioManager* mAudioManager;
		
		PlayerController* mPlayerController;
		ConfigReader mGameConfig;
		IWidgetRenderer* mWidgetRenderer;
		InputMethodManager* mInputMethodManager;

		float mDeltaTime = 0.1f;
		bool mIsRunning;

		std::function<void(float)> mTickCallback = nullptr;

		// Constructor
		GameEngine();
		~GameEngine();

		static void AtExit();

	public:
		/**
		* Creates the GameEngine.
		* This will set the global GGameEngine instance.
		* @return A pointer to the newly created GGameEngine
		*/
		static GameEngine* CreateGameEngine();

	public:
		/** Starts the game engine */
		void StartEngine();

		/** Called every frame */
		void TickEngine();

		void Shutdown();

		void SetTickCallback(std::function<void(float)> arg_callback) { mTickCallback = arg_callback; }

		/** Gets the time in seconds between current and last frame */
		float GetDeltaTime();

		void SetPlayerController(PlayerController* arg_controller) { mPlayerController = arg_controller; } // TODO

		void SetRenderTarget(IRenderTarget* arg_target) { mRenderTarget = arg_target; } // TODO

		void SetCurrentLevel(Level* arg_level);

		// Accessors
		inline Level* GetCurrentLevel() { return mCurrentLevel; }
		inline InputManager* GetInputManager() { return mInputManager; }
		inline ScriptManager* GetScriptManager() { return mScriptManager; }
		inline WidgetManager* GetWidgetManager() { return mWidgetManager; }
		inline SceneRenderer* GetSceneRenderer() { return mSceneRenderer; }
		inline AudioManager* GetAudioManager() { return mAudioManager; }
		inline ResourceManager* GetResourceManager() { return mResourceManager; }
		inline World* GetWorld() { return mWorld; }
		inline PlayerController* GetPlayerController() { return mPlayerController; }
		inline ConfigReader& GetGameConfig() { return mGameConfig; }
		inline WorldMessageBus* GetWorldMessageBus() { return mWorldMessageBus; }
		inline IWidgetRenderer* GetWidgetRenderer() { return mWidgetRenderer; }
		inline InputMethodManager* GetInputMethodManager() { return mInputMethodManager; }
		inline WindowBase* GetFocusedWindow() { return mFocusedWindow; }
	};
}

namespace Retro3D
{
	extern GameEngine* GGameEngine;
}

#endif
