#ifndef RETRO3D_CAMERACOMPONENT_H
#define RETRO3D_CAMERACOMPONENT_H

#include "component.h"
#include "transform.h"

namespace Retro3D
{
	class CameraComponent : public Component
	{
		DEFINE_CLASS(CameraComponent, Component)
	private:
		glm::vec3 mPositionOffset;
		Transform mCameraTransform;
		float mTOffset = 0.0f;
		glm::vec3 mPrevPos;

	public:
		CameraComponent();
		virtual void OnStart() override;
		virtual void OnTick(float arg_deltatime) override;

		inline const Transform& GetCameraTransform() { return mCameraTransform; }

	};
}

#endif
