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

			const float movementSpeed = deltaSeconds * 2.5f;

			InputManager* inputManager = GGameEngine->GetInputManager();

			glm::vec3 moveDist(0.0f, 0.0f, 0.0f);

			if (inputManager->GetKey("w"))
			{
				moveDist = camForward;
			}
			if (inputManager->GetKey("s"))
			{
				moveDist = camForward * -1.0f;
			}
			if (inputManager->GetKey("d"))
			{
				moveDist = camRight;
			}
			if (inputManager->GetKey("a"))
			{
				moveDist = camRight * -1.0f;
			}
			if (inputManager->GetKey("e"))
			{
				camRot *= glm::rotate(-movementSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			if (inputManager->GetKey("q"))
			{
				camRot *= glm::rotate(movementSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
			}

			const float len = glm::length(moveDist);
			if(len > 0.0f)
				camPos += glm::normalize(moveDist) * movementSpeed;

			mPlayer->GetTransform().SetPosition(camPos);
			mPlayer->GetTransform().SetRotation(camRot);

		}
	}
}
