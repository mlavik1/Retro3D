#include "emscripten_window.h"

namespace Retro3D
{
	EmscriptenWindow::EmscriptenWindow()
		: SDLWindow::SDLWindow()
	{
	}

	EmscriptenWindow::~EmscriptenWindow()
	{
		SDLWindow::~SDLWindow();
	}

	void* EmscriptenWindow::GetOSWindowHandle()
	{
		return nullptr;
	}

	bool EmscriptenWindow::HasFocus()
	{
		return true; // Only one window
	}
}
