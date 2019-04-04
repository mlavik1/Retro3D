#ifndef RETRO3D_NULLINPUTMETHODMANAGER_H
#define RETRO3D_NULLINPUTMETHODMANAGER_H

#include "Text/input_method_manager.h"

namespace Retro3D
{
	class NullInputMethodManager : public InputMethodManager
	{
	public:
		virtual void EnableInput() override {}

		virtual void DisableInput() override {}

		virtual void SetContext(ITextInputMethodContext* arg_context) override {}

		virtual bool GetString(std::string& out_string) override { return false; }

		virtual bool IsActive() override { return false; }
	};
}

#endif
