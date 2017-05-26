#include "camera_component.h"

#include "actor.h"

namespace Retro3D
{
	void CameraComponent::OnStart()
	{
		Component::OnStart();
	}

	void CameraComponent::OnTick(float arg_deltatime)
	{
		Component::OnTick(arg_deltatime);

		mCameraTransform.SetPosition(mPositionOffset + mActor->GetTransform().GetPosition());
		mCameraTransform.SetRotation(mActor->GetTransform().GetRotation());
	}
}
