#ifndef RETRO3D_GAMEENGINE_H
#define RETRO3D_GAMEENGINE_H

namespace Retro3D
{
	class Level;
	class Window;
	class InputManager;
	class SceneRenderer;

	class GameEngine
	{
	private:
		Level* mCurrentLevel;
		Window* mWindow;
		InputManager* mInputManager;
		SceneRenderer* mSceneRenderer;

		GameEngine();

	public:
		static GameEngine* CreateGameEngine();

	public:
		void TickEngine();

	public:
		inline Level* GetCurrentLevel()
		{
			return mCurrentLevel;
		}

		inline Window* GetWindow()
		{
			return mWindow;
		}

		inline InputManager* GetInputManager()
		{
			return mInputManager;
		}

		inline SceneRenderer* GetSceneRenderer()
		{
			return mSceneRenderer;
		}
	};
}

namespace Retro3D
{
	extern GameEngine* GGameEngine;
}

#endif
