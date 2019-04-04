#ifndef RETRO3D_COMPONENT_H
#define RETRO3D_COMPONENT_H

#include "Object/object.h"

namespace Retro3D
{
	class Actor;

	class Component : public Object
	{
		DEFINE_CLASS(Component, Object)
		friend class Actor;

	protected:
		Actor* mActor = nullptr;

	public:
		Component();
		~Component();

		/** Called when the game starts */
		virtual void OnStart();

		/** Called each frame */
		virtual void OnTick(float arg_deltatime);

		/** Gets the owning actor of the Component */
		inline Actor* GetActor() { return mActor; }
	};
}

#endif
