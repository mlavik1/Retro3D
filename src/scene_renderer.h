#ifndef RETRO3D_SCENERENDERER_H
#define RETRO3D_SCENERENDERER_H

#include <stdint.h>

class SDL_Surface;

namespace Retro3D
{
	class SceneRenderer
	{
	public:
		SceneRenderer();
		~SceneRenderer();

		void RenderScene();

	private:
		uint32_t getpixel(const SDL_Surface* arg_surface, int arg_x, int arg_y);
	};
}

#endif
