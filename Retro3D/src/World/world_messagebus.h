#ifndef RETRO3D_WORLDMESSAGEBUS_H
#define RETRO3D_WORLDMESSAGEBUS_H

/*=============================================================================================
World Message Bus

A "message bus" for world-related events.
When actors and components are created, they will notify WorldMessateBus.
==============================================================================================*/

#include <vector>

#include "Object/weak_objectptr.h"

namespace Retro3D
{
	class Actor;
	class Component;

	class WorldMessageBus
	{
	public:
		void ActorAdded(Actor* arg_actor);
		void ComponentAdded(Component* arg_comp);

		inline std::vector<WeakObjectPtr<Actor>>& GetActorsAdded() { return mActorsAdded; }
		inline std::vector<WeakObjectPtr<Component>>& GetComponentsAdded() { return mComponentsAdded; }

		void Clear() { mActorsAdded.clear(); mComponentsAdded.clear(); }

	private:
		std::vector<WeakObjectPtr<Actor>> mActorsAdded;
		std::vector<WeakObjectPtr<Component>> mComponentsAdded;
	};
}

#endif
