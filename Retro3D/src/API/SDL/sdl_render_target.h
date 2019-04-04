#ifndef I_RETRO3D_SDLRENDERTARGETWINDOW_H
#define I_RETRO3D_SDLRENDERTARGETWINDOW_H

#include "Graphics/render_target.h"

struct SDL_Renderer; // fwd. decl.

namespace Retro3D
{
	class ISDLRenderTarget : public IRenderTarget
	{
	public:
		virtual void PrepareRender() = 0;
		virtual void Render() = 0;
		virtual void GetRenderContextSize(int& out_width, int& out_height) = 0;
		virtual SDL_Renderer* GetSDLRenderer() = 0;
		virtual WindowBase* GetWindow() = 0;

	};
}

#endif
