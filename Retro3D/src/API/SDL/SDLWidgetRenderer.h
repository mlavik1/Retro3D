#ifndef RETRO3D_SFMLWIDGETRENDERER_H
#define RETRO3D_SFMLWIDGETRENDERER_H

#include "Window/window.h"
#include "API/SDL/sdl_render_target.h"
#include "UI/Interfaces/widget_renderer.h"
#include "Math/rectangle.h"
#include <sdl2/SDL_rect.h>
#include <unordered_map>
#include "Object/weak_objectptr.h"
#include "Resource/texture_res.h"
#include <vector>

struct SDL_Texture;

namespace Retro3D
{
	typedef std::unordered_map<WeakObjectPtr<TextureRes>, SDL_Texture*, WeakObjectPtr<TextureRes>::HashByRefHandle> TextureMapType;

	class SDLWidgetRenderer : public IWidgetRenderer
	{
	private:
		ISDLRenderTarget* mRenderTarget = 0;

		TextureMapType mSDLTextureMap;

		void InitTransform(const Rectangle& arg_contentrect, const Rectangle& arg_visiblerect, SDL_Rect& out_rect);

		TextureMapType::iterator mCurrentTextureMapIterator;

	public:
		SDLWidgetRenderer();

		void PreRender() override;

		void RenderColour(ColourVisual* arg_visual, const WidgetRenderParams& arg_renderparams) override;
		void RenderImage(ImageVisual* arg_image, const WidgetRenderParams& arg_renderparams) override;
		void RenderText(TextVisual* arg_text, const WidgetRenderParams& arg_renderparams) override;
		
		void SetRenderTarget(IRenderTarget* arg_window) override;
	};
}

#endif
