#include "component.h"

#include "st_assert.h"
#include "game_engine.h"
#include "world_messagebus.h"

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
