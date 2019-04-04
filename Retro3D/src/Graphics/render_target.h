#ifndef RETRO3D_RENDERTARGETWINDOW_H
#define RETRO3D_RENDERTARGETWINDOW_H

#include "Window/window.h"

namespace Retro3D
{
	class IRenderTarget
	{
	public:
		virtual void PrepareRender() = 0;
		virtual void Render() = 0;
		virtual void GetRenderContextSize(int& out_width, int& out_height) = 0;
		virtual WindowBase* GetWindow() = 0;
	};
}

#endif
