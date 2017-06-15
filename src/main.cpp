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
#include "actor.h"
#include "player.h"
#include "component.h"
#include "camera_component.h"
#include "st_assert.h"
#include "player_controller.h"
#include "world.h"
#include "sprite_component.h"
#include "script_component.h"

#include <typeinfo>

#include "script_manager.h"

#undef main // TEMP - TODO


int main(int argc, char** argv)
{
	LOG_INFO() << "starting from main";

	using namespace Retro3D;

	/*
	std::string obj_id = "testObj";
	
	chai.eval("class TestClass { def TestClass() {} }; var t = TestClass();");

	auto func = chai.eval<std::function<void(float)> >("fun(dt) {" + obj_id + ".on_update(dt); }");
	func(123.0f);
	*/

	GameEngine* engine = GameEngine::CreateGameEngine();


	// TODO: initialise from file
	engine->GetScriptManager()->RegisterScript("resources//chaiscript//TestClass.chai");
	engine->GetScriptManager()->RegisterScript("resources//chaiscript//TestLevel.chai");

	GGameEngine->GetCurrentLevel()->LoadLevel("level1");
	engine->GetSceneRenderer()->SetLevel(GGameEngine->GetCurrentLevel()); // todo: do in engine

	Player* player = new Player();
	player->GetTransform().SetPosition(glm::vec3(2.0f, 1.0f, 0.5f));

	CameraComponent* camComp = new CameraComponent();
	player->AddComponent(camComp);

	GGameEngine->GetPlayerController()->SetPlayer(player);

	GGameEngine->GetSceneRenderer()->SetCameraComponent(camComp); // TODO: make this automatic

	engine->StartEngine();

	SDL_Quit();

	return 0;
}
