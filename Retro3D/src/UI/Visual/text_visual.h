#ifndef RETRO3D_UI_TEXT_VISUAL_H
#define RETRO3D_UI_TEXT_VISUAL_H

/*=============================================================================================
Text Visual

A Visual that renders text.
==============================================================================================*/

#include "UI/Visual/visual.h"
#include <string>
#include "UI/Core/text_style.h"
#include "Resource/res_ptr.h"
#include "Resource/font_res.h"

namespace Retro3D
{
	class TextVisual : public Retro3D::Visual
	{
	private:
		/** The text to be rendered. */
		std::string mText;

		/** The TextStyle (font type/style/size) of the text to be rendered. */
		TextStyle mTextStyle;

		/** Font Resource, used for rendering. */
		ResPtr<FontRes> mFontRes;

	public:
		TextVisual();

		/** Gets the text of the Visual. */
		const std::string& GetText() const { return mText; }
		
		/** Sets the TextStyle of the text to be rendered. */
		TextStyle GetTextStyle() { return mTextStyle; }

		/** Sets the TextStyle of the text to be rendered. */
		void SetTextStyle(TextStyle arg_style);

		/** Sets the text to be rendered. */
		void SetText(std::string arg_text);

		/** Renders the text. */
		void RenderVisual(IWidgetRenderer *arg_renderer, const WidgetRenderParams& arg_renderparams) override;

		/** Gets the font Resource, used for rendering the text. */
		inline FontRes* GetFontRes() { return mFontRes.IsValid() ? mFontRes.Get() : nullptr; }

	};
}

#endif
