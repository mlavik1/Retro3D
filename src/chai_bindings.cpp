#include "chai_bindings.h"

#include "game_engine.h"

namespace Retro3D
{
	GameEngine* scripthelper_getengine() { return GGameEngine; }

	void ChaiBindings::AddBindings(chaiscript::ChaiScript* arg_chaiscript)
	{
		arg_chaiscript->add(chaiscript::user_type<GameEngine>(), "GameEngine");
		arg_chaiscript->add(chaiscript::fun(&GameEngine::GetDeltaTime), "GetDeltaTime");
		arg_chaiscript->add(chaiscript::fun(&scripthelper_getengine), "GetGameEngine");
	}
}
