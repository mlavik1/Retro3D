#include "rectangle.h"

namespace Retro3D
{
	Rectangle::Rectangle()
		: mPosition(0.0f, 0.0f), mSize(0.0f, 0.0f)
	{

	}

	Rectangle::Rectangle(glm::vec2 arg_position, glm::vec2 arg_size)
		: mPosition(arg_position), mSize(arg_size)
	{

	}

	Rectangle::Rectangle(float arg_x, float arg_y, float arg_width, float arg_height)
		: mPosition(arg_x, arg_y), mSize(arg_width, arg_height)
	{
		
	}
}