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
		mKeycodeMap["shift"] = SDLK_LSHIFT;
		mKeycodeMap["shift"] = SDLK_RSHIFT;
		mKeycodeMap["q"] = SDLK_q;
		mKeycodeMap["w"] = SDLK_w;
		mKeycodeMap["e"] = SDLK_e;
		mKeycodeMap["r"] = SDLK_r;
		mKeycodeMap["t"] = SDLK_t;
		mKeycodeMap["y"] = SDLK_y;
		mKeycodeMap["u"] = SDLK_u;
		mKeycodeMap["i"] = SDLK_i;
		mKeycodeMap["o"] = SDLK_o;
		mKeycodeMap["p"] = SDLK_p;
		mKeycodeMap["a"] = SDLK_a;
		mKeycodeMap["s"] = SDLK_s;
		mKeycodeMap["d"] = SDLK_d;
		mKeycodeMap["f"] = SDLK_f;
		mKeycodeMap["g"] = SDLK_g;
		mKeycodeMap["h"] = SDLK_h;
		mKeycodeMap["j"] = SDLK_j;
		mKeycodeMap["k"] = SDLK_k;
		mKeycodeMap["l"] = SDLK_l;
		mKeycodeMap["z"] = SDLK_z;
		mKeycodeMap["x"] = SDLK_x;
		mKeycodeMap["c"] = SDLK_c;
		mKeycodeMap["v"] = SDLK_v;
		mKeycodeMap["b"] = SDLK_b;
		mKeycodeMap["n"] = SDLK_n;
		mKeycodeMap["m"] = SDLK_m;

		mKeycodeMap["1"] = SDLK_1;
		mKeycodeMap["2"] = SDLK_2;
		mKeycodeMap["3"] = SDLK_3;
		mKeycodeMap["4"] = SDLK_4;
		mKeycodeMap["5"] = SDLK_5;
		mKeycodeMap["6"] = SDLK_6;
		mKeycodeMap["7"] = SDLK_7;
		mKeycodeMap["8"] = SDLK_8;
		mKeycodeMap["9"] = SDLK_9;
		mKeycodeMap["0"] = SDLK_0;

		mKeycodeMap["backspace"] = SDLK_BACKSPACE;

		for (auto keyCodePair : mKeycodeMap)
		{
			mKeycodeMapInverted[keyCodePair.second] = keyCodePair.first;
		}
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

				auto iter = mKeycodeMapInverted.find(mEvent.key.keysym.sym);
				if (iter != mKeycodeMapInverted.end())
				{
					for (IInputListener* listener : mInputListeners)
						listener->OnKeyDown(iter->second.c_str());
				}
			}
			else if (mEvent.type == SDL_KEYUP)
			{
				mKeyUpMap[mEvent.key.keysym.sym] = true;
				mKeyPressedMap[mEvent.key.keysym.sym] = false;

				auto iter = mKeycodeMapInverted.find(mEvent.key.keysym.sym);
				if (iter != mKeycodeMapInverted.end())
				{
					for (IInputListener* listener : mInputListeners)
						listener->OnKeyUp(iter->second.c_str());
				}
			}
			else if (mEvent.type == SDL_MOUSEMOTION)
			{
				mMousePosition = glm::vec2(mEvent.motion.x, mEvent.motion.y);

				for (IInputListener* listener : mInputListeners)
					listener->OnMouseMotion(glm::vec2(mEvent.motion.xrel, mEvent.motion.yrel));
			}
			else if (mEvent.type == SDL_MOUSEWHEEL)
			{
				mMouseWheel += glm::vec2(mEvent.wheel.x, mEvent.wheel.y);
			}
			else if (mEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				mMousePressStates |= (SDL_PRESSED << (mEvent.button.button - 1));

				for (IInputListener* listener : mInputListeners)
					listener->OnMouseButtonDown((MouseButtonID)(mEvent.button.button - 1));
			}
			else if (mEvent.type == SDL_MOUSEBUTTONUP)
			{
				mMouseReleaseStates |= (SDL_PRESSED << (mEvent.button.button - 1));

				for (IInputListener* listener : mInputListeners)
					listener->OnMouseButtonUp((MouseButtonID)(mEvent.button.button - 1));
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
		return (mMousePressStates & ((MouseButtonID)1 << arg_button)) != 0;
	}

	bool InputManager::GetMouseReleased(MouseButtonID arg_button)
	{
		return (mMouseReleaseStates & ((MouseButtonID)1 << arg_button)) != 0;
	}

	const glm::vec2& InputManager::GetMouseMove() const
	{
		return mMouseMove;
	}

	const glm::vec2& InputManager::GetMousePosition() const
	{
		return mMousePosition;
	}

	void InputManager::RegisterInputListener(IInputListener* arg_listener)
	{
		mInputListeners.emplace(arg_listener);
	}

	void InputManager::UnregisterInputListener(IInputListener* arg_listener)
	{
		mInputListeners.erase(arg_listener);
	}

}
