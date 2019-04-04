#ifndef RETRO3D_PLAYERCONTROLLER_H
#define RETRO3D_PLAYERCONTROLLER_H

#include "glm/glm.hpp"

namespace Retro3D
{
	class Player;

	class PlayerController
	{
	private:
		/** Checks that the specified position is a valid location that the player can move to. */
		bool IsValidMoveLocation(const glm::vec3 arg_loc);

		glm::vec3 mRequestedVelocity;
		glm::vec3 mRequestedRotation;

	protected:
		Player* mPlayer = nullptr;
		float mMovementSpeed = 2.5f;
		float mRotationSpeed = 2.5f;

		/** Consumes movement input and calculates the requested velocity and rotation. */
		void ConsumeInput();

	public:
		void SetPlayer(Player* arg_player);

		inline Player* GetPlayer() { return mPlayer; }

		virtual void OnStart();
		virtual void OnTick(float arg_deltatime);

	};
}

#endif
