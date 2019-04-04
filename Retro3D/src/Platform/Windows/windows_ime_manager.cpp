#ifdef _WIN32
#include "windows_ime_manager.h"
#include "Window/window.h"
#include "Misc/debug.h"

#pragma comment(lib, "imm32.lib")

namespace Retro3D
{
	void WindowsIMEManager::EnableInput()
	{
		if (mInputMethodContext != nullptr)
		{
			mHwnd = (HWND)mInputMethodContext->GetIMContextWindow()->GetOSWindowHandle();

			if (mHwnd)
			{
				if (CreateCaret(mHwnd, NULL, 1, 1))
				{
					mImmContext = ImmGetContext(mHwnd);
					if (mImmContext)
					{
						ImmReleaseContext(mHwnd, mImmContext);
						BOOL succeeded = ImmAssociateContextEx(mHwnd, NULL, IACE_DEFAULT);
						if (succeeded)
						{
							mInputEnabled = true;
							LOG_INFO() << "IME enabled";
						}
						else
						{
							LOG_ERROR() << "Failed to associate IME context";
						}
					}
				}
			}
		}
	}

	void WindowsIMEManager::DisableInput()
	{
		if (mHwnd && mImmContext)
		{
			BOOL succeeded = ImmAssociateContextEx(mHwnd, NULL, 0);
			if (succeeded)
			{
				mInputEnabled = false;
				LOG_INFO() << "IME disabled";
			}
		}
	}

	void WindowsIMEManager::SetContext(ITextInputMethodContext* arg_context)
	{
		mInputMethodContext = arg_context;

		if (mInputEnabled)
		{
			EnableInput();
		}
	}

	bool WindowsIMEManager::GetString(std::string& out_string)
	{

		MSG msg = {};
		BOOL hasMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

		if (!hasMessage)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		::SetCaretPos(500, 200);

		// Try get result string
		int stringSize = ::ImmGetCompositionString(mImmContext, GCS_RESULTSTR, NULL, 0);

		if (stringSize > 0)
		{
			int stringLength = stringSize / sizeof(wchar_t);

			// increase buffer size for terminating null character,  
			stringSize += sizeof(WCHAR);

			HGLOBAL hstr = GlobalAlloc(GHND, stringSize);
			if (hstr == NULL)
			{
			}

			LPSTR lpstr = (LPSTR)GlobalLock(hstr);
			if (lpstr == NULL)
			{
			}

			// Get the result strings into lpstr
			ImmGetCompositionString(mImmContext, GCS_RESULTSTR, lpstr, stringSize);
			
			ImmSetCompositionString(mImmContext, SCS_SETSTR, lpstr, stringSize, NULL, 0);
			
			ImmReleaseContext(mHwnd, mImmContext);

			std::string tmpStr = std::string(lpstr);

			// TODO: Fix this hack!
			int size_needed = WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)lpstr, (int)tmpStr.size(), NULL, 0, NULL, NULL);
			std::string strTo(size_needed, 0);
			WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)lpstr, (int)tmpStr.size(), &strTo[0], size_needed, NULL, NULL);

			out_string = strTo.substr(0, strTo.find((char)0));

			GlobalUnlock(hstr);
			GlobalFree(hstr);
			return true;
		}

		return false;
	}

	bool WindowsIMEManager::IsActive()
	{
		return mInputEnabled;
	}

}

#endif
