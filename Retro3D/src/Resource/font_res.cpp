#include "font_res.h"

#include "Misc/debug.h"
#include "Engine/game_engine.h"

namespace Retro3D
{
	bool FontRes::LoadResource(const std::string& arg_path)
	{
		std::string path = arg_path;
		int fontSize = 12;
		// extract font size from path parameter (if specified) - TODO: add optional params parameter?
		for (size_t i = 0; i < arg_path.length(); i++)
		{
			const char& currChar = arg_path[i];
			if (currChar == ':')
			{
				int newFontSize = std::strtol(&arg_path[i + 1], nullptr, 10);
				if (newFontSize)
				{
					fontSize = newFontSize;
					path = arg_path.substr(0, i);
					break;
				}
			}
		}

		mFont = TTF_OpenFont(path.c_str(), fontSize);

		if (mFont == nullptr)
		{
			LOG_ERROR() << "Error loading font: " << TTF_GetError();
		}

		return (mFont != nullptr);
	}

	FontRes::~FontRes()
	{
		if (mFont != nullptr)
		{
			TTF_CloseFont(mFont);
		}
	}
}
