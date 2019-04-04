#include "component.h"

#include "Misc/st_assert.h"
#include "Engine/game_engine.h"
#include "World/world_messagebus.h"

IMPLEMENT_CLASS(Retro3D::Component)

namespace Retro3D
{
	Component::Component()
	{
		GGameEngine->GetWorldMessageBus()->ComponentAdded(this);
	}

	Component::~Component()
	{

	}

	void Component::OnStart()
	{
		__AssertComment(mActor != nullptr, "Component is not attached to an actor");
	}

	void Component::OnTick(float arg_deltatime)
	{

	}
}
