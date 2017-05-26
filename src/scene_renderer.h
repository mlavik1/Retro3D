#ifndef RETRO3D_SCENERENDERER_H
#define RETRO3D_SCENERENDERER_H

#include <stdint.h>
#include <unordered_map>
#include "objectptr.h"

class SDL_Surface;

namespace Retro3D
{
	class Level;
	class CameraComponent; // TODO: This is bad!

	class SceneRenderer
	{
	public:
		SceneRenderer();
		~SceneRenderer();

		void RenderScene();
		void SetLevel(Level* arg_level);
		void SetCameraComponent(CameraComponent* arg_comp); // TEMP

	private:
		uint32_t getpixel(const SDL_Surface* arg_surface, int arg_x, int arg_y);
		Level* mLevel;
		std::unordered_map<char, SDL_Surface*> mTextureSurfaceMap;
		SDL_Surface* mSkyboxTexture = nullptr;
		ObjectPtr<CameraComponent> mCameraComponent; // TODO: pass on some other structure
	};
}

#endif
