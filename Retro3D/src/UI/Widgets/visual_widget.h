#ifndef RETRO3D_VISUALWIDGET_H
#define RETRO3D_VISUALWIDGET_H

/*=============================================================================================
A visible Widget, with a background colour.

==============================================================================================*/

#include "widget.h"
#include "UI/Visual/colour_visual.h"

namespace Retro3D
{
	class VisualWidget : public Widget
	{
	protected:
		ColourVisual mBackgroundColourVisual;

	public:
		VisualWidget();

		void SetBackgroundColour(Colour &arg_colour);

		virtual void CreateContent() override;

	};
}

#endif
