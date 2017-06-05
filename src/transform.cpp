#include "transform.h"

namespace Retro3D
{
	void Transform::SetPosition(glm::vec3 arg_pos)
	{
		mPosition = arg_pos;
	}

	void Transform::SetRotation(glm::mat4 arg_rot)
	{
		mRotation = arg_rot;
	}

	glm::vec3 Transform::GetForwardVector() const
	{
		return mRotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	}

	glm::vec3 Transform::GetRightVector() const
	{
		return mRotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	}

	glm::vec3 Transform::GetUpVector() const
	{
		return mRotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	}
}
