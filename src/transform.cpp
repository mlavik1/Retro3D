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
}
