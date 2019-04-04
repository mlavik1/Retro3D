#ifdef _WIN32
#include "windows_window.h"
#include <iostream>
#include <sdl2/SDL_syswm.h>

namespace Retro3D
{
	WindowsWindow::WindowsWindow()
		: SDLWindow::SDLWindow()
	{
	}

	WindowsWindow::~WindowsWindow()
	{
		SDLWindow::~SDLWindow();
	}

	void* WindowsWindow::GetOSWindowHandle()
	{
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(GetSDLWindow(), &wmInfo);
		HWND hWnd = wmInfo.info.win.window;
		return hWnd;
	}

	bool WindowsWindow::HasFocus()
	{
		return GetActiveWindow() == GetOSWindowHandle();
	}
}

#endif
