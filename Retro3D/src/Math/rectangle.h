#ifndef RETRO3D_RECTANGLE_H
#define RETRO3D_RECTANGLE_H

#include "glm/glm.hpp"

namespace Retro3D
{
	class Rectangle
	{
	public:
		glm::vec2 mPosition;
		glm::vec2 mSize;

		Rectangle();
		Rectangle(glm::vec2 arg_position, glm::vec2 arg_size);
		Rectangle(float arg_x, float arg_y, float arg_width, float arg_height);
	};
}

#endif
