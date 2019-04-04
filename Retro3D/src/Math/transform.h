#ifndef RETRO3D_TRANSFORM_H
#define RETRO3D_TRANSFORM_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace Retro3D
{
	class Transform
	{
	private:
		glm::vec3 mPosition;
		glm::mat4 mRotation;

	public:
		inline const glm::vec3& GetPosition() const { return mPosition; }
		inline const glm::mat4& GetRotation() const { return mRotation; }

		void SetPosition(glm::vec3 arg_pos);
		void SetRotation(glm::mat4 arg_rot);

		glm::vec3 GetForwardVector() const;
		glm::vec3 GetRightVector() const;
		glm::vec3 GetUpVector() const;
	};
}

#endif
