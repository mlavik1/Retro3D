#ifdef _WIN32
#ifndef RETRO3D_IMEMANAGER_H
#define RETRO3D_IMEMANAGER_H

#include "Text/input_method_manager.h"
#include "Text/text_input_method_context.h"
#include <Windows.h>

namespace Retro3D
{
	class WindowsIMEManager : public InputMethodManager
	{
	private:
		HWND mHwnd = NULL;
		HIMC mImmContext = NULL;
		ITextInputMethodContext* mInputMethodContext = nullptr;
		bool mInputEnabled = false;

	public:
		virtual void EnableInput() override;
		virtual void DisableInput() override;

		virtual void SetContext(ITextInputMethodContext* arg_context) override;

		virtual bool GetString(std::string& out_string) override;

		virtual bool IsActive();
	};
}

#endif
#endif
