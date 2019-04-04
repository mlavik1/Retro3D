#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "sdl2/SDL.h"
#include "sdl2/sdl_image.h"
#include <iostream>
#include <vector>
#include <math.h>

#include "Engine/game_engine.h"
#include "Graphics/scene_renderer.h"
#include "World/level.h"
#include "Actor/player.h"
#include "Component/component.h"
#include "Component/camera_component.h"
#include "Misc/st_assert.h"
#include "Actor/player_controller.h"
#include "Audio/audio_manager.h"
#include "Audio/audio_track.h"

#undef main // TEMP - TODO

using namespace Retro3D;


int main(int argc, char** argv)
{
	LOG_INFO() << "starting from main";

#ifdef RETRO3D_OPTIMISATIONS_DISABLED
	LOG_INFO() << "Optimisations are disabled!";
#endif

	GameEngine* engine = GameEngine::CreateGameEngine();

	engine->StartEngine();

	return 0;
}
