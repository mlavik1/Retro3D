#include "visual_widget.h"
#include "UI/Interfaces/widget_renderer.h"

namespace Retro3D
{
	VisualWidget::VisualWidget()
	{
		mBackgroundColourVisual.SetColour(Colour(0.7f, 0.7f, 0.7f, 0.0f));
	}

	void VisualWidget::CreateContent()
	{
		AddVisual(&mBackgroundColourVisual);
	}

	void VisualWidget::SetBackgroundColour(Colour& arg_colour)
	{
		mBackgroundColourVisual.SetColour(arg_colour);
	}
}
