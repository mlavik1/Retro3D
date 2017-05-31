#include "input_manager.h"

namespace Retro3D
{

	InputManager::InputManager()
	{
		SetupInputMap();
	}

	void InputManager::SetupInputMap()
	{
		mKeycodeMap["up"] = SDLK_UP;
		mKeycodeMap["down"] = SDLK_DOWN;
		mKeycodeMap["left"] = SDLK_LEFT;
		mKeycodeMap["right"] = SDLK_RIGHT;
		mKeycodeMap["space"] = SDLK_SPACE;
		mKeycodeMap["return"] = SDLK_RETURN;
		mKeycodeMap["w"] = SDLK_w;
		mKeycodeMap["a"] = SDLK_a;
		mKeycodeMap["s"] = SDLK_s;
		mKeycodeMap["d"] = SDLK_d;
		mKeycodeMap["q"] = SDLK_q;
		mKeycodeMap["e"] = SDLK_e;
		mKeycodeMap["1"] = SDLK_1;
		mKeycodeMap["2"] = SDLK_2;
	}

	void InputManager::CaptureInput()
	{
		mKeyUpMap.clear();
		mKeyDownMap.clear();
		mMouseReleaseStates = 0;
		mMouseMove = glm::vec2(0.0f, 0.0f);

		while (SDL_PollEvent(&mEvent))
		{
			//GUIManager::Instance()->HandleInput(mEvent);

			if (mEvent.type == SDL_KEYDOWN)
			{
				mKeyDownMap[mEvent.key.keysym.sym] = true;
				mKeyPressedMap[mEvent.key.keysym.sym] = true;
			}
			else if (mEvent.type == SDL_KEYUP)
			{
				mKeyUpMap[mEvent.key.keysym.sym] = true;
				mKeyPressedMap[mEvent.key.keysym.sym] = false;
			}
			else if (mEvent.type == SDL_MOUSEMOTION)
			{
				mMousePosition = glm::vec2(mEvent.motion.x, mEvent.motion.y);
			}
			else if (mEvent.type == SDL_MOUSEWHEEL)
			{
				mMouseWheel += glm::vec2(mEvent.wheel.x, mEvent.wheel.y);
			}
			else if (mEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				mMousePressStates |= (SDL_PRESSED << (mEvent.button.button - 1));
			}
			else if (mEvent.type == SDL_MOUSEBUTTONUP)
			{
				mMouseReleaseStates |= (SDL_PRESSED << (mEvent.button.button - 1));
			}


		}

	}


	bool InputManager::GetKey(const char* arg_key)
	{
		auto itKeyCode = mKeycodeMap.find(arg_key);
		if (itKeyCode != mKeycodeMap.end())
		{
			const InputMapKeyCode& keyCode = (*itKeyCode).second;
			auto itKeyPress = mKeyPressedMap.find(keyCode);
			if (itKeyPress != mKeyPressedMap.end())
				return (*itKeyPress).second;
		}
		return false;
	}

	bool InputManager::GetKeyDown(const char* arg_key)
	{
		auto itKeyCode = mKeycodeMap.find(arg_key);
		if (itKeyCode != mKeycodeMap.end())
		{
			const InputMapKeyCode& keyCode = (*itKeyCode).second;
			auto itKeyDown = mKeyDownMap.find(keyCode);
			if (itKeyDown != mKeyDownMap.end())
				return (*itKeyDown).second;
		}
		return false;
	}

	bool InputManager::GetKeyUp(const char* arg_key)
	{
		auto itKeyCode = mKeycodeMap.find(arg_key);
		if (itKeyCode != mKeycodeMap.end())
		{
			const InputMapKeyCode& keyCode = (*itKeyCode).second;
			auto itKeyUp = mKeyUpMap.find(keyCode);
			if (itKeyUp != mKeyUpMap.end())
				return (*itKeyUp).second;
		}
		return false;
	}

	bool InputManager::GetMousePressed(MouseButtonID arg_button)
	{
		return mMousePressStates & ((MouseButtonID)1 << arg_button);
	}

	bool InputManager::GetMouseReleased(MouseButtonID arg_button)
	{
		return mMouseReleaseStates & ((MouseButtonID)1 << arg_button);
	}

	const glm::vec2& InputManager::GetMouseMove() const
	{
		return mMouseMove;
	}

	const glm::vec2& InputManager::GetMousePosition() const
	{
		return mMousePosition;
	}

}
