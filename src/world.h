#ifndef RETRO3D_WORLD_H
#define RETRO3D_WORLD_H

#include <vector>
#include "objectptr.h"

namespace Retro3D
{
	class Actor;

	class World
	{
	private:
		std::vector<ObjectPtr<Actor>> mActors;

	public:

		void AddActor(Actor* arg_actor);

		inline const std::vector<ObjectPtr<Actor>>& GetActors() { return mActors; }
	};
}

#endif
