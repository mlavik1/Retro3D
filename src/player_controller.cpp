#include "player_controller.h"
#include "input_manager.h"

#include "game_engine.h"
#include "player.h"

namespace Retro3D
{
	void PlayerController::SetPlayer(Player* arg_player)
	{
		mPlayer = arg_player;
	}

	void PlayerController::OnStart()
	{

	}

	void PlayerController::OnTick(float arg_deltatime)
	{
		if (mPlayer)
		{
			const float deltaSeconds = arg_deltatime;

			glm::vec3 camPos = mPlayer->GetTransform().GetPosition();
			glm::mat4 camRot = mPlayer->GetTransform().GetRotation();

			const glm::vec3 camForward = camRot * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			const glm::vec3 camRight = camRot * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

			const float movementSpeed = deltaSeconds * 3.0f;

			InputManager* inputManager = GGameEngine->GetInputManager();

			if (inputManager->GetKey("w"))
			{
				camPos += camForward * movementSpeed;
			}
			if (inputManager->GetKey("s"))
			{
				camPos -= camForward * movementSpeed;
			}
			if (inputManager->GetKey("d"))
			{
				camPos += camRight * movementSpeed;
			}
			if (inputManager->GetKey("a"))
			{
				camPos -= camRight * movementSpeed;
			}
			if (inputManager->GetKey("e"))
			{
				camRot *= glm::rotate(-movementSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			if (inputManager->GetKey("q"))
			{
				camRot *= glm::rotate(movementSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
			}

			mPlayer->GetTransform().SetPosition(camPos);
			mPlayer->GetTransform().SetRotation(camRot);

		}
	}
}
