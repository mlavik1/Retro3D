#ifndef RETRO3D_CAMERACOMPONENT_H
#define RETRO3D_CAMERACOMPONENT_H

#include "component.h"
#include "transform.h"

namespace Retro3D
{
	class CameraComponent : public Component
	{
	private:
		glm::vec3 mPositionOffset;
		Transform mCameraTransform;

	public:
		virtual void OnStart() override;
		virtual void OnTick(float arg_deltatime) override;

		inline const Transform& GetCameraTransform() { return mCameraTransform; }

	};
}

#endif
