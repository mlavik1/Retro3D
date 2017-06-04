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

#include <typeinfo>

#undef main // TEMP - TODO

int main(int argc, char** argv)
{
	LOG_INFO() << "starting from main";

	using namespace Retro3D;

	GameEngine* engine = GameEngine::CreateGameEngine();

	GGameEngine->GetCurrentLevel()->LoadLevel("level1");
	engine->GetSceneRenderer()->SetLevel(GGameEngine->GetCurrentLevel()); // todo: do in engine

	Player* player = new Player();
	player->GetTransform().SetPosition(glm::vec3(2.0f, 1.0f, 0.5f));

	CameraComponent* camComp = new CameraComponent();
	player->AddComponent(camComp);
	__Assert(player->GetComponent<CameraComponent>() == camComp);

	GGameEngine->GetPlayerController()->SetPlayer(player);

	Actor* actor1 = new Actor();

	SpriteComponent* spriteComp = new SpriteComponent();
	spriteComp->SetTexture("resources//textures//grass1.png");
	actor1->AddComponent(spriteComp);
	actor1->GetTransform().SetPosition(glm::vec3(2.5f, 4.0f, 0.5f));


	GGameEngine->GetSceneRenderer()->SetCameraComponent(camComp); // TODO: make this automatic

	engine->StartEngine();

	SDL_Quit();

	return 0;
}
