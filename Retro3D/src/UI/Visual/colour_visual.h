#ifndef RETRO3D_UI_COLOUR_VISUAL_H
#define RETRO3D_UI_COLOUR_VISUAL_H

/*=============================================================================================
Colour Visual

A Visual that renders a coloured rectangle.
==============================================================================================*/

#include "UI/Visual/visual.h"
#include "UI/Core/colour.h"

namespace Retro3D
{
	class ColourVisual : public Visual
	{
	private:
		Colour mColour;

	public:
		ColourVisual();

		inline const Colour& GetColour() { return mColour; }
		void SetColour(Colour arg_colour);

		/** Renders the visual. */
		void RenderVisual(IWidgetRenderer *arg_renderer, const WidgetRenderParams& arg_renderparams);
	};
}

#endif
