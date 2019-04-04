#include "Actor/actor.h"
#include "Component/component.h"

#include "Engine/game_engine.h"
#include "World/world_messagebus.h"

namespace Retro3D
{
	Actor::Actor()
	{
		GGameEngine->GetWorldMessageBus()->ActorAdded(this);
	}

	Actor::~Actor()
	{

	}

	void Actor::AddComponent(Component* comp)
	{
		mComponents.push_back(comp);
		comp->mActor = this;
	}

	void Actor::OnStart()
	{
		// do not call OnSTart() on components, since this is done from world.cpp
	}

	void Actor::OnTick(float arg_deltatime)
	{
		for (ObjectPtr<Component> comp : mComponents)
		{
			comp->OnTick(arg_deltatime);
		}
	}
}
