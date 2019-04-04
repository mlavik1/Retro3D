#include "texture_res.h"

#include "Misc/debug.h"

namespace Retro3D
{
	bool TextureRes::LoadResource(const std::string& arg_path)
	{
		mSDLSurface = IMG_Load(arg_path.c_str());

		return (mSDLSurface != nullptr);
	}

	
	TextureRes::~TextureRes()
	{
		if (mSDLSurface != nullptr)
		{
			SDL_FreeSurface(mSDLSurface);
		}
	}
}
