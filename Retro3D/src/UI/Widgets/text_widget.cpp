#include "text_widget.h"

#include "Engine/game_engine.h"

namespace Retro3D
{
	TextWidget::TextWidget()
	{
		mTextVisual.SetText("");
		AddVisual(&mTextVisual);
	}

	std::string TextWidget::GetText()
	{
		return mTextVisual.GetText();
	}

	void TextWidget::SetText(const std::string &arg_string)
	{
		mTextVisual.SetText(arg_string);
	}

	TextStyle TextWidget::GetTextStyle()
	{
		return mTextVisual.GetTextStyle();
	}

	void TextWidget::SetTextStyle(TextStyle arg_style)
	{
		mTextVisual.SetTextStyle(arg_style);
	}

	void TextWidget::CreateContent()
	{

	}
}
