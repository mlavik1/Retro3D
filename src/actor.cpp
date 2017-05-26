#include "actor.h"
#include "component.h"

namespace Retro3D
{
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
