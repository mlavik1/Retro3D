#include "image_visual.h"

#include "UI/Interfaces/widget_renderer.h"
#include "Engine/game_engine.h"
#include "Window/window.h"
#include "Resource/resource_manager.h"
#include "API/SDL/sdl_render_target.h"
#include "UI/Managers/WidgetManager.h"

namespace Retro3D
{
	
	ImageVisual::ImageVisual()
	{

	}

	ImageVisual::~ImageVisual()
	{
	}

	void ImageVisual::RenderVisual(IWidgetRenderer *arg_renderer, const WidgetRenderParams& arg_renderparams)
	{
		arg_renderer->RenderImage(this, arg_renderparams);
	}

	void ImageVisual::SetImagePath(std::string arg_img)
	{
		if (mImagePath != arg_img)
		{
			mImagePath = arg_img;
			// TODO: don't do that here
			mImageRes = GGameEngine->GetResourceManager()->LoadResource<TextureRes>(mImagePath);
		}
	}
}
