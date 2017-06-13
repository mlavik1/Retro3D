#include "game_engine.h"

#include "input_manager.h"
#include "scene_renderer.h"
#include "level.h"
#include "window.h"
#include "world.h"
#include <sdl2/SDL.h>
#include "actor.h"
#include "player_controller.h"
#include "world_messagebus.h"
#include "script_manager.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace Retro3D
{
	GameEngine* GGameEngine = nullptr;

	GameEngine::GameEngine()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		atexit(SDL_Quit);
		GGameEngine = this;
		mInputManager = new InputManager();
		mScriptManager = new ScriptManager();
		mWindow = new Window();
		mSceneRenderer = new SceneRenderer();
		mWorldMessageBus = new WorldMessageBus();
		mWorld = new World();
		mCurrentLevel = new Level();
		mPlayerController = new PlayerController();
		if (!mGameConfig.ReadFile("resources//config//GameConfig.ini"))
			LOG_ERROR() << "Failed to read config game file: ";
	}

	GameEngine* GameEngine::CreateGameEngine()
	{
		LOG_INFO() << "Creating game engine";
		GGameEngine = new GameEngine();
		return GGameEngine;
	}

#ifdef __EMSCRIPTEN__
	void emscriptenMainLoop()
	{
		GGameEngine->TickEngine();
	}
#endif

	void GameEngine::StartEngine()
	{
		LOG_INFO() << "Starting game engine";
		mPlayerController->OnStart();

		for (const ObjectPtr<Actor> actor : mWorld->GetActors())
		{
			actor->OnStart();
		}

		LOG_INFO() << "Entering main loop";

		mIsRunning = true;

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(emscriptenMainLoop, 60, 1);
#else
		while (mIsRunning) // TODO
		{
			TickEngine();
		}
#endif
	}

	void GameEngine::TickEngine()
	{
		const Uint64 start = SDL_GetPerformanceCounter();

		mWorld->TickWorld(mDeltaTime);
		mWorldMessageBus->Clear();

		mInputManager->CaptureInput();

		mPlayerController->OnTick(mDeltaTime);
		for (const ObjectPtr<Actor> actor : mWorld->GetActors())
		{
			actor->OnTick(mDeltaTime);
		}

		mSceneRenderer->RenderScene();


		const Uint64 end = SDL_GetPerformanceCounter();
		const static Uint64 freq = SDL_GetPerformanceFrequency();
		mDeltaTime = (end - start) / static_cast< float >(freq);
		//std::cout << "Frame time: " << mDeltaTime * 1000.0 << "ms" << std::endl;
	}

	float GameEngine::GetDeltaTime()
	{
		return mDeltaTime;
	}

}
