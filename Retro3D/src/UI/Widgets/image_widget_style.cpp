#include "image_widget_style.h"

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
		mImagePath = arg_path;
	}

	void ImageWidgetStyle::SetColour(Colour arg_colour)
	{
		mColour = arg_colour;
	}
}
