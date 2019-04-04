#include "world.h"

#include "Actor/actor.h"
#include "Engine/game_engine.h"
#include "world_messagebus.h"
#include "Component/component.h"

namespace Retro3D
{
	World::World()
	{
		mComponentMap = std::unordered_map<const Retro3D::Class*, std::vector<Component*>>();
	}

	void World::addActor(Actor* arg_actor)
	{
		mActors.push_back(arg_actor);
	}

	void World::addComponent(Component* arg_component)
	{
		const Retro3D::Class* compClass = arg_component->GetClass();
		auto iter = mComponentMap.find(compClass);
		if (iter == mComponentMap.end())
			mComponentMap[compClass].push_back(arg_component);
		else
			(*iter).second.push_back(arg_component);
	}

	void World::TickWorld(float arg_deltatime)
	{
		for (ObjectPtrBase<Actor> actor : GGameEngine->GetWorldMessageBus()->GetActorsAdded())
		{
			addActor(actor.Get());
			actor->OnStart();
		}
		for (ObjectPtrBase<Component> comp : GGameEngine->GetWorldMessageBus()->GetComponentsAdded())
		{
			addComponent(comp.Get());
			comp->OnStart();
		}
	}
}
