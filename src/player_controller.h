#ifndef RETRO3D_PLAYERCONTROLLER_H
#define RETRO3D_PLAYERCONTROLLER_H

namespace Retro3D
{
	class Player;

	class PlayerController
	{
	protected:
		Player* mPlayer = nullptr;

	public:
		void SetPlayer(Player* arg_player);

		inline Player* GetPlayer() { return mPlayer; }

		virtual void OnStart();
		virtual void OnTick(float arg_deltatime);

	};
}

#endif
