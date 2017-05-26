#include "game_engine.h"

#include "input_manager.h"
#include "scene_renderer.h"
#include "level.h"
#include "window.h"
#include "world.h"
#include <sdl2/SDL.h>
#include "actor.h"
#include "player_controller.h"

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
		mWorld = new World();
		mCurrentLevel = new Level();
		mPlayerController = new PlayerController();
	}

	GameEngine* GameEngine::CreateGameEngine()
	{
		GGameEngine = new GameEngine();
		return GGameEngine;
	}

	void GameEngine::StartEngine()
	{
		mPlayerController->OnStart();

		for (const ObjectPtr<Actor> actor : mWorld->GetActors())
		{
			actor->OnStart();
		}

		float deltaTime = 0.1f;

		while (true) // TODO
		{
			const Uint64 start = SDL_GetPerformanceCounter();


			mInputManager->CaptureInput();

			mPlayerController->OnTick(deltaTime);
			for (const ObjectPtr<Actor> actor : mWorld->GetActors())
			{
				actor->OnTick(deltaTime);
			}

			mSceneRenderer->RenderScene();


			const Uint64 end = SDL_GetPerformanceCounter();
			const static Uint64 freq = SDL_GetPerformanceFrequency();
			deltaTime = (end - start) / static_cast< float >(freq);
			std::cout << "Frame time: " << deltaTime * 1000.0 << "ms" << std::endl;
		}
	}

}
