#ifndef RETRO3D_WIDGET_RENDER_PARAMS_H
#define RETRO3D_WIDGET_RENDER_PARAMS_H

#include "Math/rectangle.h"

namespace Retro3D
{
	class WidgetRenderParams
	{
	public:
		/**
		* Defines the position and extent of the content to be rendered.
		* This is decided by the transform of the widget.
		*/
		Rectangle mContentRect;

		/**
		* Defines visible region of the content to be rendered.
		* This is decided by the transform of the parent widget.
		*/
		Rectangle mVisibleRect;
	};
}

#endif
