#ifdef _WIN32
#ifndef RETRO3D_WINDOWS_WINDOW_H
#define RETRO3D_WINDOWS_WINDOW_H

/*=============================================================================================
Game Window class for Windows builds.

==============================================================================================*/

#include "API/SDL/sdl_window.h"

namespace Retro3D
{
	class WindowsWindow : public SDLWindow
	{
	public:
		WindowsWindow();
		virtual ~WindowsWindow() override;

		virtual void* GetOSWindowHandle() override;

		virtual bool HasFocus() override;
	};
}

#endif
#endif
