#include "actor.h"
#include "component.h"

#include "game_engine.h"
#include "world_messagebus.h"

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
		for (Component* comp : mComponents)
		{
			comp->OnStart();
		}
	}

	void Actor::OnTick(float arg_deltatime)
	{
		for (Component* comp : mComponents)
		{
			comp->OnTick(arg_deltatime);
		}
	}
}
