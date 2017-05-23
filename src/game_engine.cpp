#include "game_engine.h"

#include "input_manager.h"
#include "scene_renderer.h"
#include "level.h"
#include "window.h"
#include <sdl2/SDL.h>

namespace Retro3D
{
	GameEngine* GGameEngine = nullptr;

	GameEngine::GameEngine()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		atexit(SDL_Quit);
		GGameEngine = this;
		mInputManager = new InputManager();
		mWindow = new Window();
		mSceneRenderer = new SceneRenderer();
		mCurrentLevel = new Level();
	}

	GameEngine* GameEngine::CreateGameEngine()
	{
		GGameEngine = new GameEngine();
		return GGameEngine;
	}

	void GameEngine::TickEngine()
	{
		mInputManager->CaptureInput();
		mSceneRenderer->RenderScene();
	}

}
