#include "texture.h"

#include "Misc/debug.h"
#include <SDL2/SDL_image.h>

namespace Retro3D
{
	Texture::Texture()
	{

	}

	Texture::~Texture()
	{
		
	}

	bool Texture::LoadTexture(const char* arg_file)
	{
		mSDLSurface = IMG_Load(arg_file);
		if (mSDLSurface == nullptr)
		{
			LOG_WARNING() << "Failed to load texture: " << arg_file;
			return false;
		}
		return true;
	}
}
