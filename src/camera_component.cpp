#include "camera_component.h"

#include "actor.h"

namespace Retro3D
{
	IMPLEMENT_CLASS(CameraComponent)

	CameraComponent::CameraComponent()
	{

	}

	void CameraComponent::OnStart()
	{
		Component::OnStart();
	}

	void CameraComponent::OnTick(float arg_deltatime)
	{
		Component::OnTick(arg_deltatime);

		const glm::vec3 velocity = mActor->GetTransform().GetPosition() - mPrevPos;
		const float magnitude = glm::length(velocity);
		if (magnitude > 0.0f)
		{
			mTOffset += arg_deltatime;
			mPositionOffset.z = cosf(mTOffset) * 0.0f; // TODO
		}

		mCameraTransform.SetPosition(mPositionOffset + mActor->GetTransform().GetPosition());
		mCameraTransform.SetRotation(mActor->GetTransform().GetRotation());
		
		mPrevPos = mActor->GetTransform().GetPosition();
	}
}
