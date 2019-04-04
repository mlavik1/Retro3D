#ifndef RETRO3D_INPUTMANAGER_H
#define RETRO3D_INPUTMANAGER_H

/*=============================================================================================
Input Manager

Handles mouse and keyboard input.

==============================================================================================*/

#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <set>
#include <string>
#include "Engine/input_listener.h"

namespace Retro3D
{
	typedef SDL_Keycode InputMapKeyCode;
	typedef unsigned int MouseButtonID;

	class InputManager
	{
	public:
		InputManager();

		/**
		* Checks if the specified key is pressed down. 
		* @return true, if arg_key is pressed down.
		*/
		bool GetKey(const char* arg_key);
		
		/**
		* Checks if the specified key was pressed down this frame.
		* @return true, if arg_key was pressed down.
		*/
		bool GetKeyDown(const char* arg_key);
		
		/**
		* Checks if the specified key was pressed up this frame.
		* @return true, if arg_key was pressed up.
		*/
		bool GetKeyUp(const char* arg_key);

		// std::string versions of the GetKey-functions (used by ChaiScript)
		bool GetKey_String(const std::string& arg_key) { return GetKey(arg_key.c_str()); }
		bool GetKeyDown_String(const std::string& arg_key) { return GetKeyDown(arg_key.c_str()); }
		bool GetKeyUp_String(const std::string& arg_key) { return GetKeyUp(arg_key.c_str()); }

		/**
		* Checks if the specified mouse button is pressed.
		* @return true, if arg_button is pressed.
		*/
		bool GetMousePressed(MouseButtonID arg_button);
		
		/**
		* Checks if the specified mouse button was released this frame.
		* @return true, if arg_button was released.
		*/
		bool GetMouseReleased(MouseButtonID arg_button);

		/** Returns the movement vector of the mouse for the current frame. */
		const glm::vec2& GetMouseMove() const;

		/** Returns the absolute position of the mouse. */
		const glm::vec2& GetMousePosition() const;

		/** Registers an input listener. Input events will be forwarded to the listener. */
		void RegisterInputListener(IInputListener* arg_listener);

		/** Unregisters an input listener. */
		void UnregisterInputListener(IInputListener* arg_listener);

		/** Sets up the input mappings for SDL. Called by the engine. */
		void SetupInputMap();

		/** Captures mouse and keyboard input. Called by the engine. */
		void CaptureInput();

	private:
		std::unordered_map<std::string, InputMapKeyCode> mKeycodeMap;
		std::unordered_map<InputMapKeyCode, std::string> mKeycodeMapInverted;

		std::unordered_map<InputMapKeyCode, bool> mKeyDownMap;
		std::unordered_map<InputMapKeyCode, bool> mKeyUpMap;
		std::unordered_map<InputMapKeyCode, bool> mKeyPressedMap;

		MouseButtonID mMousePressStates;
		MouseButtonID mMouseReleaseStates;

		glm::vec2 mMouseMove;
		glm::vec2 mMousePosition;
		glm::vec2 mMouseWheel;

		SDL_Event mEvent;

		std::set<IInputListener*> mInputListeners;
	};
}

#endif
