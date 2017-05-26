#ifndef RETRO3D_ACTOR_H
#define RETRO3D_ACTOR_H

#include "object.h"
#include "transform.h"
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace Retro3D
{
	class Component;

	class Actor : public Object
	{
	protected:
		Transform mTransform;
		std::vector<Component*> mComponents; // TODO: RTTI

	public:
		inline Transform& GetTransform() { return mTransform; }

		void AddComponent(Component* comp);

		template<typename T>
		T* GetComponent()
		{
			for (Component* comp : mComponents)
			{
				T* t = dynamic_cast<T*>(comp);
				if (t)
					return t;
			}
		}

		virtual void OnStart();
		virtual void OnTick(float arg_deltatime);

	};
}

#endif
