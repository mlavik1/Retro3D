#include "sdl2/SDL.h"
#include "sdl2/sdl_image.h"
#include "Misc/debug.h"

#include "Engine/game_engine.h"

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
