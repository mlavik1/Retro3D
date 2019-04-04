#ifndef RETRO3D_IMAGEWIDGET_H
#define RETRO3D_IMAGEWIDGET_H

/*=============================================================================================
An image Widget, for rendering an image and/or a colour.

The image will be rendered on top of the colour.

==============================================================================================*/

#include "visual_widget.h"
#include "image_widget_style.h"
#include "UI/Visual/colour_visual.h"
#include "UI/Visual/image_visual.h"

namespace Retro3D
{
	class ImageWidget : public VisualWidget
	{
	protected:
		/** Colour Visual, for rendering the background colour. */
		ObjectPtr<ColourVisual> mColourVisual;

		/** Image Visual, for rendering the image. */
		ObjectPtr<ImageVisual> mImageVisual;

		/** Image Widget Style, containing ImageVisual and ColourVisual */
		ImageWidgetStyle mImageWidgetStyle;

		/** Removes all Visuals and add them back again. */
		void setupVisuals();

	public:
		ImageWidget();

		/** Sets the path to the image file to be rendered. */
		void SetImagePath(const std::string &arg_path);

		/** Sets the colour to be rendered */
		void SetColour(Colour arg_colour);

		virtual void CreateContent() override;

		void SetImageWidgetStyle(ImageWidgetStyle& arg_style);

		inline ImageWidgetStyle GetImageWidgetStyle() { return mImageWidgetStyle; }

	};
}

#endif
