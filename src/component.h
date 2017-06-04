#ifndef RETRO3D_COMPONENT_H
#define RETRO3D_COMPONENT_H

#include "object.h"

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
		virtual void OnStart();
		virtual void OnTick(float arg_deltatime);

		inline Actor* GetActor() { return mActor; }
	};
}

#endif
