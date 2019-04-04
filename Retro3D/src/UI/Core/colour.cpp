#include "colour.h"

namespace Retro3D
{
	Colour::Colour()
		: mRGBA(0.0f, 0.0f, 0.0f, 0.0f)
	{

	}

	Colour::Colour(glm::vec4 arg_colour)
		: mRGBA(arg_colour)
	{

	}

	Colour::Colour(glm::vec3 arg_colour)
		: Colour::Colour(glm::vec4(arg_colour, 1.0f))
	{

	}

	Colour::Colour(float arg_r, float arg_g, float arg_b, float arg_a)
		: mRGBA(arg_r, arg_g, arg_b, arg_a)
	{

	}

	Colour::Colour(float arg_r, float arg_g, float arg_b)
		: Colour::Colour(arg_r, arg_g, arg_b, 1.0f)
	{

	}

	bool Colour::IsVisible() const
	{
		return mRGBA.a > 0.0f;
	}
}
