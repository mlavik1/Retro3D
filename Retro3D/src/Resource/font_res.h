#ifndef RETRO3D_FONTRES_H
#define RETRO3D_FONTRES_H

#include "resource.h"
#include <SDL2/SDL_ttf.h>

namespace Retro3D
{
	class FontRes : public Resource
	{
	private:
		TTF_Font* mFont = nullptr;

	public:
		~FontRes();

		virtual bool LoadResource(const std::string& arg_path) override;

		inline TTF_Font* GetFont() { return mFont; }
	};
}

#endif
