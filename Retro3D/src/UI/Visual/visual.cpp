#include "UI/Visual/visual.h"

namespace Retro3D
{
	void Visual::RenderVisual(IWidgetRenderer *arg_renderer, const WidgetRenderParams& arg_renderparams)
	{

	}

	void Visual::Activate(bool arg_active)
	{
		mActivated = arg_active;
	}
}