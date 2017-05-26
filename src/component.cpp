#include "component.h"

#include "st_assert.h"

namespace Retro3D
{
	void Component::OnStart()
	{
		__AssertComment(mActor != nullptr, "Component is not attached to an actor");
	}

	void Component::OnTick(float arg_deltatime)
	{

	}
}
