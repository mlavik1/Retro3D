#ifndef RETRO3D_TEXTURERES_H
#define RETRO3D_TEXTURERES_H

#include "resource.h"
#include <SDL2/SDL_image.h>

namespace Retro3D
{
	class TextureRes : public Resource
	{
	private:
		SDL_Surface* mSDLSurface = nullptr;

	public:
		~TextureRes() override;

		virtual bool LoadResource(const std::string& arg_path) override;

		inline SDL_Surface* GetSDLSurface() { return mSDLSurface; }
	};
}

#endif
