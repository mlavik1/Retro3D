#ifndef RETRO3D_TEXTINPUTWIDGET_H
#define RETRO3D_TEXTINPUTWIDGET_H

#include "text_widget.h"
#include "Text/text_input_method_context.h"

/*=============================================================================================
A Text Widget, for rendering text.

==============================================================================================*/

namespace Retro3D
{
	class TextInputWidget : public TextWidget, public ITextInputMethodContext
	{
	public:
		TextInputWidget();

		void AddText(std::string arg_text);

		virtual void OnKeyUp(const char* arg_key) override;

		virtual WindowBase* GetIMContextWindow() override;
		virtual void HandleTextInputMethodResult(std::string arg_input) override;

	};
}

#endif
