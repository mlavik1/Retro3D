#ifndef RETRO3D_TEXTINPUTMETHODCONTEXT_H
#define RETRO3D_TEXTINPUTMETHODCONTEXT_H

#include <string>

namespace Retro3D
{
	class WindowBase;

	class ITextInputMethodContext
	{
	public:
		virtual WindowBase* GetIMContextWindow() = 0;
		virtual void HandleTextInputMethodResult(std::string arg_input) = 0;
	};
}

#endif
