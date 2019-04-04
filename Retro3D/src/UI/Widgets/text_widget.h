#ifndef RETRO3D_TEXTWIDGET_H
#define RETRO3D_TEXTWIDGET_H

#include "visual_widget.h"
#include "UI/Visual/text_visual.h"
#include "Text/text_input_method_context.h"

/*=============================================================================================
A Text Widget, for rendering text.

==============================================================================================*/

namespace Retro3D
{
	class TextWidget : public VisualWidget
	{
	protected:
		TextVisual mTextVisual;

	public:
		TextWidget();

		std::string GetText();
		void SetText(const std::string &arg_string);

		TextStyle GetTextStyle();
		void SetTextStyle(TextStyle arg_style);

		virtual void CreateContent() override;
	};
}

#endif
