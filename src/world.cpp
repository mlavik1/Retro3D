#include "world.h"

#include "actor.h"

namespace Retro3D
{
	void World::AddActor(Actor* arg_actor)
	{
		mActors.push_back(arg_actor);
	}
}
