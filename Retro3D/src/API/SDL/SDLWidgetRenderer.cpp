#include "SDLWidgetRenderer.h"
#include "UI/Visual/text_visual.h"
#include "UI/Visual/colour_visual.h"
#include "UI/Visual/image_visual.h"
#include "UI/Core/text_style.h"
#include "Misc/debug.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "Engine/game_engine.h"
#include "UI/Managers/WidgetManager.h"

#include <math.h>

namespace Retro3D
{
	SDLWidgetRenderer::SDLWidgetRenderer()
	{
		mCurrentTextureMapIterator = mSDLTextureMap.end();
	}

	void SDLWidgetRenderer::PreRender()
	{
		// Delete cached textures no longer in use
		// TODO: Test this by changing image paths of image widgets at runtime
		// TODO: Support same TextureRes in multiple render targets
		for (size_t i = 0; i < std::fminl(10, mSDLTextureMap.size()); i++)
		{
			if (mCurrentTextureMapIterator == mSDLTextureMap.end())
			{
				mCurrentTextureMapIterator = mSDLTextureMap.begin();
			}

			WeakObjectPtr<TextureRes> textureRes = mCurrentTextureMapIterator->first;
			if (!textureRes.IsValid())
			{
				SDL_Texture* sdlTexture = mCurrentTextureMapIterator->second;
				if (sdlTexture != nullptr)
				{
					SDL_DestroyTexture(sdlTexture);
				}
				auto oldIter = mCurrentTextureMapIterator;
				mCurrentTextureMapIterator++;
				mSDLTextureMap.erase(oldIter);
				continue;
			}

			mCurrentTextureMapIterator++;
		}
	}

	void SDLWidgetRenderer::RenderColour(ColourVisual* arg_visual, const WidgetRenderParams& arg_renderparams)
	{
		const glm::vec4& rgba = arg_visual->GetColour().GetRGBA();
		int window_width;
		int window_height;
		mRenderTarget->GetRenderContextSize(window_width, window_height);

		SDL_Renderer* renderer = mRenderTarget->GetSDLRenderer();
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, static_cast<int>(rgba.r * 255.0f), static_cast<int>(rgba.g * 255.0f), static_cast<int>(rgba.b * 255.0f), static_cast<int>(rgba.a * 255.0f));
		SDL_Rect rect;
		rect.x = static_cast<int>(arg_renderparams.mVisibleRect.mPosition.x);
		rect.y = static_cast<int>(arg_renderparams.mVisibleRect.mPosition.y);
		rect.w = static_cast<int>(arg_renderparams.mVisibleRect.mSize.x);
		rect.h = static_cast<int>(arg_renderparams.mVisibleRect.mSize.y);
		SDL_RenderFillRect(renderer, &rect);
	}

	void SDLWidgetRenderer::RenderImage(ImageVisual* arg_image, const WidgetRenderParams& arg_renderparams)
	{
		SDL_Renderer* renderer = mRenderTarget->GetSDLRenderer();

		int window_width;
		int window_height;
		mRenderTarget->GetRenderContextSize(window_width, window_height);

		TextureRes* textureRes = arg_image->GetImageRes();

		if (textureRes == nullptr)
		{
			LOG_ERROR() << "No texture res for: " << arg_image->GetImagePath();
			return;
		}

		auto sdlTextureIter = mSDLTextureMap.find(textureRes);
		SDL_Texture* img;
		if (sdlTextureIter != mSDLTextureMap.end())
		{
			img = sdlTextureIter->second;
		}
		else
		{
			ISDLRenderTarget* renderTarget = dynamic_cast<ISDLRenderTarget*>(GGameEngine->GetWidgetManager()->GetCurrentRenderingRenderTarget());
			if (renderTarget)
			{
				img = SDL_CreateTextureFromSurface(renderTarget->GetSDLRenderer(), textureRes->GetSDLSurface());
				mSDLTextureMap[textureRes] = img;
			}
		}

		if (img == nullptr)
		{
			LOG_ERROR() << "Image visual has no loaded texture: " << arg_image->GetImagePath();
			return;
		}
		int w, h;
		SDL_QueryTexture(img, NULL, NULL, &w, &h);

		const glm::vec2 relPos = (arg_renderparams.mVisibleRect.mPosition - arg_renderparams.mContentRect.mPosition) / arg_renderparams.mContentRect.mSize;
		const glm::vec2 srcPos = glm::vec2(w * relPos.x, h * relPos.y);
		const glm::vec2 relSize = arg_renderparams.mVisibleRect.mSize / arg_renderparams.mContentRect.mSize;
		const glm::vec2 srcSize = glm::vec2(w * relSize.x, h * relSize.y);

		SDL_Rect srcRect;
		srcRect.x = static_cast<int>(relPos.x * w);
		srcRect.y = static_cast<int>(relPos.y * h);
		srcRect.w = static_cast<int>(relSize.x * w);
		srcRect.h = static_cast<int>(relSize.y * h);

		SDL_Rect dstRect;
		dstRect.x = static_cast<int>(arg_renderparams.mVisibleRect.mPosition.x);
		dstRect.y = static_cast<int>(arg_renderparams.mVisibleRect.mPosition.y);
		dstRect.w = static_cast<int>(arg_renderparams.mVisibleRect.mSize.x);
		dstRect.h = static_cast<int>(arg_renderparams.mVisibleRect.mSize.y);

		SDL_RenderCopy(renderer, img, &srcRect, &dstRect);
	}


	void SDLWidgetRenderer::RenderText(TextVisual* arg_text, const WidgetRenderParams& arg_renderparams)
	{
		const TextStyle& style = arg_text->GetTextStyle();
		const bool wrapText = style.GetWrapText();
		const std::string& text = arg_text->GetText();

		if (text.length() <= 0)
			return; // nothing to render

		int window_width;
		int window_height;
		mRenderTarget->GetRenderContextSize(window_width, window_height);

		SDL_Renderer* renderer = mRenderTarget->GetSDLRenderer();

		SDL_Rect rect;
		rect.x = static_cast<int>(arg_renderparams.mVisibleRect.mPosition.x);
		rect.y = static_cast<int>(arg_renderparams.mVisibleRect.mPosition.y);
		rect.w = static_cast<int>(arg_renderparams.mVisibleRect.mSize.x);
		rect.h = static_cast<int>(arg_renderparams.mVisibleRect.mSize.y);

		TTF_Font* font = arg_text->GetFontRes() ? arg_text->GetFontRes()->GetFont() : nullptr;

		if (font == nullptr)
		{
			LOG_ERROR() << "Failed to open font for text: " << arg_text->GetText().c_str();
			return;
		}

		SDL_Color White = { 255, 255, 255 };

		SDL_Surface* surface;
		if(wrapText)
			surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), White, (Uint32)rect.w);
		else
			surface = TTF_RenderUTF8_Blended(font, text.c_str(), White);

		const int iW = surface->w;
		const int iH = surface->h;

		if (iW < rect.w)
		{
			rect.x += (rect.w - iW) / 2;
			rect.w = iW;
		}
		if (iH < rect.h)
		{
			rect.y += (rect.h - iH) / 2;
			rect.h = iH;
		}

		SDL_Texture* txtTexture = SDL_CreateTextureFromSurface(renderer, surface);

		const glm::vec2 relPos = (arg_renderparams.mVisibleRect.mPosition - arg_renderparams.mContentRect.mPosition) / arg_renderparams.mContentRect.mSize;
		const glm::vec2 srcPos = glm::vec2(iW * relPos.x, iH * relPos.y);
		const glm::vec2 relSize = arg_renderparams.mVisibleRect.mSize / arg_renderparams.mContentRect.mSize;
		const glm::vec2 srcSize = glm::vec2(iW * relSize.x, iH * relSize.y);

		SDL_Rect srcRect;
		srcRect.x = static_cast<int>(relPos.x * iW);
		srcRect.y = static_cast<int>(relPos.y * iH);
		srcRect.w = static_cast<int>(relSize.x * iW);
		srcRect.h = static_cast<int>(relSize.y * iH);

		SDL_RenderCopy(renderer, txtTexture, &srcRect, &rect);

		SDL_FreeSurface(surface); // TODO: use resource manager
		SDL_DestroyTexture(txtTexture); // TODO: use resource manager
	}
	
	void SDLWidgetRenderer::SetRenderTarget(IRenderTarget* arg_window)
	{
		mRenderTarget = (ISDLRenderTarget*)arg_window;
		__Assert(mRenderTarget);
	}
	
	
	void SDLWidgetRenderer::InitTransform(const Rectangle& arg_contentrect, const Rectangle& arg_visiblerect, SDL_Rect& out_rect)
	{
		int window_width;
		int window_height;
		mRenderTarget->GetRenderContextSize(window_width, window_height);

		SDL_Renderer* renderer = mRenderTarget->GetSDLRenderer();
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		out_rect.x = static_cast<int>(arg_contentrect.mPosition.x);
		out_rect.y = static_cast<int>(arg_contentrect.mPosition.y);
		out_rect.w = static_cast<int>(arg_contentrect.mSize.x);
		out_rect.h = static_cast<int>(arg_contentrect.mSize.y);
	}

}