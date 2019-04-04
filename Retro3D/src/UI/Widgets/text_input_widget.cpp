#include "text_input_widget.h"

#include "Engine/game_engine.h"
#include "Engine/input_manager.h"
#include "Text/input_method_manager.h"

namespace Retro3D
{
	TextInputWidget::TextInputWidget()
	{
		TextWidget::TextWidget();
	}

	void TextInputWidget::AddText(std::string arg_text)
	{
		std::string txt = GetText();
		txt = txt + arg_text;
		SetText(txt);
	}

	void TextInputWidget::OnKeyUp(const char* arg_key)
	{
		TextWidget::OnKeyUp(arg_key);

		std::string txt(arg_key);
		if (txt == "backspace") // TEMP
		{
			// TODO: Support erasing UTF-encoded characters wider than one byte
			std::string txt = GetText();
			txt = txt.substr(0, txt.length() - 1);
			SetText(txt);
		}

		else
		{
			if (GGameEngine->GetInputMethodManager()->IsActive())
			{
				return; // IME input
			}

			if (txt == "space")
			{
				AddText(" ");
			}
			else if (txt.length() == 1)
			{
				std::string newTxt = txt;

				if (GGameEngine->GetInputManager()->GetKey("shift"))
				{
					newTxt[0] = std::toupper(newTxt[0]);
				}
				AddText(newTxt);
			}
		}
	}

	WindowBase* TextInputWidget::GetIMContextWindow()
	{
		return GGameEngine->GetFocusedWindow();
	}

	void TextInputWidget::HandleTextInputMethodResult(std::string arg_input)
	{
		AddText(arg_input);
	}

}
