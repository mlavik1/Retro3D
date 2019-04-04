#ifndef RETRO3D_UI_VISUAL_H
#define RETRO3D_UI_VISUAL_H

/*=============================================================================================
									UIVisual base class

Base class for UI Visuals, such as text and images.
==============================================================================================*/

#include "UI/Core/widget_render_params.h"
#include "Object/object.h"

namespace Retro3D
{
	class IWidgetRenderer;

	class Visual : public Object
	{
	protected:
		bool mActivated = true;

	public:
		/**
		* This is called during UI rendering.
		* Subclasses of Widget render their contents through the WidgetRenderer here.
		*/
		virtual void RenderVisual(IWidgetRenderer *arg_renderer, const WidgetRenderParams& arg_renderparams);

		/** Activates the Visual. The Visual will be rendered next frame. */
		void Activate(bool arg_active);

		/** Checks if the Visual is activated, and will be rendered. */
		inline bool IsActivated() { return mActivated; }
	};
}

#endif
