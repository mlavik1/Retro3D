#include "text_visual.h"

#include "UI/Interfaces/widget_renderer.h"
#include "Engine/game_engine.h"
#include "Resource/resource_manager.h"

namespace Retro3D
{
	TextVisual::TextVisual()
	{
		SetTextStyle(mTextStyle); // to load the resource
	}

	void TextVisual::SetTextStyle(TextStyle arg_style)
	{
		if (!mFontRes.IsValid() || arg_style.GetFontName() != mTextStyle.GetFontName() || arg_style.GetFontSize() != mTextStyle.GetFontSize())
		{
			std::string resourceParam = arg_style.GetFontName() + std::string(":") + std::to_string(arg_style.GetFontSize());

			ResPtr<FontRes> loadedRes = GGameEngine->GetResourceManager()->LoadResource<FontRes>(resourceParam);
			if (loadedRes.IsValid())
			{
				mTextStyle = arg_style;
				mFontRes = loadedRes;
			}
			else
			{
				LOG_ERROR() << "Failed to load font: " << arg_style.GetFontName();
			}
		}
	}

	void TextVisual::SetText(std::string arg_text)
	{
		mText = arg_text;
	}

	void TextVisual::RenderVisual(IWidgetRenderer *arg_renderer, const WidgetRenderParams& arg_renderparams)
	{
		arg_renderer->RenderText(this, arg_renderparams);
	}
}