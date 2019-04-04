#ifndef RETRO3D_SDL_WINDOW_H
#define RETRO3D_SDL_WINDOW_H

/*=============================================================================================
Game Window class for SDL.

The Game, with its scene and GUI, is rendered here.

==============================================================================================*/

#include "Graphics/render_target.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "API/SDL/sdl_render_target.h"
#include "Window/window.h"

namespace Retro3D
{
	class SDLWindow : public ISDLRenderTarget, public WindowBase
	{
	private:
		SDL_Window* mSDLWindow;
		SDL_Renderer* mSDLRenderer;

	public:
		SDLWindow();
		virtual ~SDLWindow() override;

		/** Prepare the Window for rendering. Called by the engine. */
		virtual void PrepareRender() override;

		/** Updates the screen with rendered content. */
		virtual void Render() override;

		inline SDL_Window* GetSDLWindow() { return mSDLWindow; }

		virtual SDL_Renderer* GetSDLRenderer() override;

		/** Gets the size (in pixels) of the render context. */
		virtual void GetRenderContextSize(int& out_width, int& out_height) override;

		/** Gets the size (in pixels) of the Window. */
		virtual void GetWindowSize(int& out_width, int& out_height) override;

		virtual void* GetOSWindowHandle() = 0;

		virtual WindowBase* GetWindow() override;
	};
}

#endif
