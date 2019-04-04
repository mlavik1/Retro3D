#ifndef RETRO3D_INPUTMETHODMANAGER_H
#define RETRO3D_INPUTMETHODMANAGER_H

#include "Text/text_input_method_context.h"
#include <string>

namespace Retro3D
{
	class InputMethodManager
	{
	public:
		virtual void EnableInput() = 0;

		virtual void DisableInput() = 0;

		virtual void SetContext(ITextInputMethodContext* arg_context) = 0;

		virtual bool GetString(std::string& out_string) = 0;

		virtual bool IsActive() = 0;
	};
}

#endif
