#ifndef RETRO3D_EMSCRIPTEN_WINDOW_H
#define RETRO3D_EMSCRIPTEN_WINDOW_H

/*=============================================================================================
Game Window class for Emscripten builds.

==============================================================================================*/

#include "API/SDL/sdl_window.h"

namespace Retro3D
{
	class EmscriptenWindow : public SDLWindow
	{
	public:
		EmscriptenWindow();
		virtual ~EmscriptenWindow() override;

		virtual void* GetOSWindowHandle() override;

		virtual bool HasFocus() override;
	};
}

#endif
