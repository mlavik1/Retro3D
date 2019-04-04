#ifndef RETRO3D_COLOUR_H
#define RETRO3D_COLOUR_H

#include "glm/glm.hpp"

namespace Retro3D
{
	class Colour
	{
	private:
		glm::vec4 mRGBA;

	public:
		Colour();
		Colour(glm::vec4 arg_colour);
		Colour(glm::vec3 arg_colour);
		Colour(float arg_r, float arg_g, float arg_b, float arg_a);
		Colour(float arg_r, float arg_g, float arg_b);

		inline const glm::vec4& GetRGBA() const { return mRGBA; }

		bool IsVisible() const;
	};
}

#endif
