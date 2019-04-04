#ifndef RETRO3D_IMAGEWIDGETSTYLE_H
#define RETRO3D_IMAGEWIDGETSTYLE_H

/*=============================================================================================
The style of an image Widget.

Has an image and a colour.

==============================================================================================*/

#include "Object/objectptr.h"
#include <string>
#include "UI/Core/colour.h"

namespace Retro3D
{
	class ImageWidgetStyle
	{
	private:
		std::string mImagePath;
		Colour mColour;

	public:
		ImageWidgetStyle();
		~ImageWidgetStyle();

		/** Sets the path to the image file to be rendered. */
		void SetImagePath(const std::string &arg_path);

		/** Sets the colour to be rendered */
		void SetColour(Colour arg_colour);

		const inline std::string GetImagePath() { return mImagePath; }
		const inline Colour GetColour() { return mColour; }

	};
}

#endif
