#ifndef RETRO3D_INPUTMANAGER_H
#define RETRO3D_INPUTMANAGER_H

#include "sdl2/SDL.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <string>

namespace Retro3D
{
	typedef SDL_Keycode InputMapKeyCode;
	typedef unsigned int MouseButtonID;

	class InputManager
	{
	public:
		InputManager();

		void SetupInputMap();
		void CaptureInput();

		bool GetKey(const char* arg_key);
		bool GetKeyDown(const char* arg_key);
		bool GetKeyUp(const char* arg_key);

		// std::string versions (used by ChaiScript)
		bool GetKey_String(const std::string& arg_key) { return GetKey(arg_key.c_str()); }
		bool GetKeyDown_String(const std::string& arg_key) { return GetKeyDown(arg_key.c_str()); }
		bool GetKeyUp_String(const std::string& arg_key) { return GetKeyUp(arg_key.c_str()); }

		bool GetMousePressed(MouseButtonID arg_button);
		bool GetMouseReleased(MouseButtonID arg_button);

		const glm::vec2& GetMouseMove() const;
		const glm::vec2& GetMousePosition() const;

	private:
		std::unordered_map<std::string, InputMapKeyCode> mKeycodeMap;

		std::unordered_map<InputMapKeyCode, bool> mKeyDownMap;
		std::unordered_map<InputMapKeyCode, bool> mKeyUpMap;
		std::unordered_map<InputMapKeyCode, bool> mKeyPressedMap;

		MouseButtonID mMousePressStates;
		MouseButtonID mMouseReleaseStates;

		glm::vec2 mMouseMove;
		glm::vec2 mMousePosition;
		glm::vec2 mMouseWheel;

		SDL_Event mEvent;
	};
}

#endif
