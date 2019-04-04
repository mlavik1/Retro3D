#ifndef RETRO3D_WIDGET_RENDERER_H
#define RETRO3D_WIDGET_RENDERER_H

/*=============================================================================================
                              Widget Renderer Interface

 For each low-level implementation of UI renderring (drawing images, fonts, etc.)
  you will inherit from this interface. Libraries such as SFML and SDL will have their own versions.
==============================================================================================*/

#include "UI/Core/widget_render_params.h"

namespace Retro3D
{
	class ImageVisual;
	class TextVisual;
	class ColourVisual;
	class Window;
	class IRenderTarget;

	class IWidgetRenderer
	{
	public:
		virtual void PreRender() = 0;
		virtual void RenderColour(ColourVisual* arg_rect, const WidgetRenderParams& arg_renderparams) = 0;
		virtual void RenderImage(ImageVisual* arg_image, const WidgetRenderParams& arg_renderparams) = 0;
		virtual void RenderText(TextVisual* arg_image, const WidgetRenderParams& arg_renderparams) = 0;
		virtual void SetRenderTarget(IRenderTarget* arg_window) = 0;
	};
}

#endif
