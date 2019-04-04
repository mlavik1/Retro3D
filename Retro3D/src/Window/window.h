#ifndef RETRO3D_WINDOWBASE_H
#define RETRO3D_WINDOWBASE_H

/*=============================================================================================
Base class for windows.

==============================================================================================*/

namespace Retro3D
{
	class WindowBase
	{
	public:
		virtual ~WindowBase() = 0;

		/** Gets the size (in pixels) of the Window. */
		virtual void GetWindowSize(int& out_width, int& out_height) = 0;

		virtual void* GetOSWindowHandle() = 0;

		virtual bool HasFocus() = 0;
	};
}

#endif
