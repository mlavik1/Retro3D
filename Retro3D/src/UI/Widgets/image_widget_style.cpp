#include "image_widget_style.h"
#include "Engine/game_engine.h"
#include "Misc/path_utils.h"

namespace Retro3D
{
	ImageWidgetStyle::ImageWidgetStyle()
	{
		
	}

	ImageWidgetStyle::~ImageWidgetStyle()
	{

	}

	void ImageWidgetStyle::SetImagePath(const std::string &arg_path)
	{
        mImagePath = PathUtils::CombinePaths(GGameEngine->GetProjectDirectory(), arg_path);
	}

	void ImageWidgetStyle::SetColour(Colour arg_colour)
	{
		mColour = arg_colour;
	}
}
