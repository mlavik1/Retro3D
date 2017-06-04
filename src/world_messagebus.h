#ifndef RETRO3D_WORLDMESSAGEBUS_H
#define RETRO3D_WORLDMESSAGEBUS_H

#include <vector>

namespace Retro3D
{
	class Actor;
	class Component;

	class WorldMessageBus
	{
	public:
		void ActorAdded(Actor* arg_actor);
		void ComponentAdded(Component* arg_comp);

		inline std::vector<Actor*>& GetActorsAdded() { return mActorsAdded; }
		inline std::vector<Component*>& GetComponentsAdded() { return mComponentsAdded; }

		void Clear() { mActorsAdded.clear(); mComponentsAdded.clear(); }

	private:
		std::vector<Actor*> mActorsAdded;
		std::vector<Component*> mComponentsAdded;
	};
}

#endif
