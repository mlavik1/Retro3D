#include "SDL2/SDL.h"
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

    std::string projectPath = "";
    if (argc > 1)
        projectPath = argv[1];

    // TEMP TEST
    //projectPath = "projects/samples/maze"; // TODO: REMOVE ME

	GameEngine* engine = GameEngine::CreateGameEngine(projectPath);

	engine->StartEngine();
    
	return 0;
}
