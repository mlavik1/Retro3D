#include "colour_visual.h"

#include "UI/Interfaces/widget_renderer.h"

namespace Retro3D
{
	ColourVisual::ColourVisual()
	{
		SetColour(Colour(0.0f, 0.0f, 0.0f, 0.0f));
	}

	void ColourVisual::SetColour(Colour arg_colour)
	{
		mColour = arg_colour;
	}

	void ColourVisual::RenderVisual(IWidgetRenderer *arg_renderer, const WidgetRenderParams& arg_renderparams)
	{
		arg_renderer->RenderColour(this, arg_renderparams);
	}

}
