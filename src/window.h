#ifndef RETRO3D_WINDOW_H
#define RETRO3D_WINDOW_H

#include <sdl2/SDL.h>
#include <sdl2/SDL_video.h>

namespace Retro3D
{
	class Window
	{
	private:
		SDL_Window* mSDLWindow;
		SDL_Renderer* mSDLRenderer;

	public:
		Window();
		~Window();

		inline SDL_Window* GetSDLWindow()
		{
			return mSDLWindow;
		}

		inline SDL_Renderer* GetSDLRenderer()
		{
			return mSDLRenderer;
		}
	};
}

#endif
