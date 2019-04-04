#ifndef RETRO3D_WORLD_H
#define RETRO3D_WORLD_H

/*=============================================================================================
World

Contains actors and components, and is responsible for ticking them.
==============================================================================================*/

#include <vector>
#include "Object/objectptr.h"
#include <unordered_map>

namespace Retro3D
{
	class Actor;
	class Component;

	class World
	{
	private:
		std::vector<ObjectPtr<Actor>> mActors;
		std::unordered_map<const Retro3D::Class*, std::vector<Component*>> mComponentMap;

		void addActor(Actor* arg_actor); // TODO: use a message bus
		void addComponent(Component* arg_component); // TODO: use a message bus

	public:
		World();

		void TickWorld(float arg_deltatime);

		inline const std::vector<ObjectPtr<Actor>>& GetActors() { return mActors; }

		template<typename T>
		const std::vector<T*> GetComponentsOfType()
		{
			const Retro3D::Class* compClass = T::GetStaticClass();
			std::vector<T*> outVector;
			auto iter = mComponentMap.find(compClass);
			if (iter != mComponentMap.end())
			{
				for (Component* comp : (*iter).second)
					outVector.push_back(static_cast<T*>(comp));
			}
			return outVector;
		}
	};
}

#endif
