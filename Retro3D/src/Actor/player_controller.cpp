#include "player_controller.h"

#include "Engine/input_manager.h"
#include "Engine/game_engine.h"
#include "player.h"
#include "World/level.h"

namespace Retro3D
{
    PlayerController::PlayerController()
    {

    }

	void PlayerController::SetPlayer(Player* arg_player)
	{
		mPlayer = arg_player;
	}

    void PlayerController::AddInput(const glm::vec3& dir)
    {
        mRequestedVelocity += dir;
    }

    void PlayerController::AddRotation(const glm::vec3& rot)
    {
        mRequestedRotation += rot;
    }

    void PlayerController::Move(const glm::vec3& dir)
    {
        AddInput(dir);
        HandleMovement();
    }

    void PlayerController::Rotate(const glm::vec3& rot)
    {
        AddRotation(rot);
        HandleMovement();
    }

	void PlayerController::OnStart()
	{

	}

	void PlayerController::OnTick(float arg_deltatime)
	{
        HandleMovement();
	}

    void PlayerController::HandleMovement()
    {
        if (mPlayer)
        {
            const glm::vec3& playerPos = mPlayer->GetTransform().GetPosition();
            const glm::mat4& playerRot = mPlayer->GetTransform().GetRotation();

            // Is our current position an invalid location? If so, we need to allow getting out of it
            const bool initialOverlap = !IsValidMoveLocation(playerPos);

            if (mRequestedVelocity.x != 0.0f || mRequestedVelocity.y != 0.0f || mRequestedVelocity.z != 0.0f)
            {
                const glm::vec3 moveDir = glm::normalize(mRequestedVelocity);
                glm::vec3 desiredDeltaMove = mRequestedVelocity;
                float desiredDeltaMoveLength = glm::length(desiredDeltaMove);

                const float MOVE_STEP_LENGTH = 0.1f;

                glm::vec3 newPos = playerPos;
                // Move player by MOVE_STEP_LENGTH, until target location reached or move rejected.
                // TODO: This scales badly with move distance
                while (desiredDeltaMoveLength > 0.0f)
                {
                    float currentMoveLength = glm::min(desiredDeltaMoveLength, MOVE_STEP_LENGTH);
                    desiredDeltaMoveLength -= currentMoveLength;
                    glm::vec3 moveDist = moveDir * currentMoveLength;

                    // If target position is not a valid location, try move only along the X-axis or Y-axis (if possible)
                    if (!initialOverlap && !IsValidMoveLocation(newPos + moveDist))
                    {
                        const bool xMoveValid = IsValidMoveLocation(newPos + glm::vec3(moveDist.x, 0.0f, 0.0f));
                        const bool yMoveValid = IsValidMoveLocation(newPos + glm::vec3(0.0f, moveDist.y, 0.0f));

                        if (xMoveValid)
                        {
                            moveDist = glm::vec3(moveDist.x, 0.0f, 0.0f);
                        }
                        else if (yMoveValid)
                        {
                            moveDist = glm::vec3(0.0f, moveDist.y, 0.0f);
                        }
                        else
                        {
                            moveDist = glm::vec3(0.0f, 0.0f, 0.0f);
                            break;
                        }
                    }

                    newPos += moveDist;
                }
                // Update player position
                mPlayer->GetTransform().SetPosition(newPos);
            }

            // Update player rotation
            if (glm::length(mRequestedRotation) > 0.0f)
            {
                glm::mat4 rotMat = glm::rotate(glm::length(mRequestedRotation), mRequestedRotation);;
                mPlayer->GetTransform().SetRotation(playerRot * rotMat);
            }
        }
        mRequestedVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
        mRequestedRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    }

	bool PlayerController::IsValidMoveLocation(const glm::vec3 arg_loc)
	{
		const float DESIRED_WALL_DISTANCE = 0.15f;
		Level* level = GGameEngine->GetCurrentLevel();

		const int intX = (int)arg_loc.x;
		const int intY = (int)arg_loc.y;
		const float fractX = arg_loc.x - intX;
		const float fractY = arg_loc.y - intY;

		const bool crossRight = fractX + DESIRED_WALL_DISTANCE > 1.0f;
		const bool crossLeft = fractX - DESIRED_WALL_DISTANCE < 0.0f;
		const bool crossFwd = fractY + DESIRED_WALL_DISTANCE > 1.0f;
		const bool crossBack = fractY - DESIRED_WALL_DISTANCE < 0.0f;

		// Make sure location is on a walkable floor, and that it's not too close to a nearby unwalkable floor
		if (!level->IsWalkableFloor(intX, intY)
			|| crossRight && !level->IsWalkableFloor(intX + 1, intY)
			|| crossLeft && !level->IsWalkableFloor(intX - 1, intY)
			|| crossFwd && !level->IsWalkableFloor(intX, intY + 1)
			|| crossBack && !level->IsWalkableFloor(intX, intY - 1)
			|| crossRight && crossFwd && !level->IsWalkableFloor(intX + 1, intY + 1)
			|| crossRight && crossBack && !level->IsWalkableFloor(intX + 1, intY - 1)
			|| crossLeft && crossFwd && !level->IsWalkableFloor(intX - 1, intY + 1)
			|| crossLeft && crossBack && !level->IsWalkableFloor(intX - 1, intY - 1))
		{
			return false;
		}
		return true;
	}

}
