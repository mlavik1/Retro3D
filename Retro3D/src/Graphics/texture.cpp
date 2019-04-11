#include "texture.h"

#include "Misc/debug.h"
#include <SDL2/SDL_image.h>
#include "Engine/game_engine.h"
#include "Misc/path_utils.h"

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
        std::string file = PathUtils::CombinePaths(GGameEngine->GetProjectDirectory(), arg_file);
		mSDLSurface = IMG_Load(file.c_str());
		if (mSDLSurface == nullptr)
		{
			LOG_WARNING() << "Failed to load texture: " << file;
			return false;
		}
		return true;
	}
}
