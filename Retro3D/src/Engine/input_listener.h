#ifndef RETRO3D_INPUTLISTENER_H
#define RETRO3D_INPUTLISTENER_H

namespace Retro3D
{
	typedef unsigned int MouseButtonID;

	class IInputListener
	{
	public:
		virtual void OnKeyDown(const char* arg_key) {};
		virtual void OnKeyUp(const char* arg_key) {};
		virtual void OnMouseButtonDown(MouseButtonID arg_button) {};
		virtual void OnMouseButtonUp(MouseButtonID arg_button) {};
		virtual void OnMouseMotion(const glm::vec2& arg_motion) {};
	};
};

#endif
