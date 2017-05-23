#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "sdl2/SDL.h"
#include "sdl2/sdl_image.h"
#include <iostream>
#include <vector>
#include <math.h>

#include "game_engine.h"
#include "scene_renderer.h"
#include "level.h"

#undef main // TEMP - TODO

int main(int argc, char** argv)
{
	using namespace Retro3D;

	GameEngine* engine = GameEngine::CreateGameEngine();

	GGameEngine->GetCurrentLevel()->LoadLevel("level1");

	while (true)
	{
		engine->TickEngine();
	}

	SDL_Quit();

	return 0;
}
