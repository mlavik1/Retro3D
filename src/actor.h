#ifndef RETRO3D_ACTOR_H
#define RETRO3D_ACTOR_H

#include "object.h"
#include "transform.h"
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <string>

namespace Retro3D
{
	class Component;

	class Actor : public Object
	{
	protected:
		Transform mTransform;
		std::vector<Component*> mComponents; // TODO: RTTI

	private:
		bool mHasStarted = false;
		std::string mActorName = "";

	public:
		Actor();
		~Actor();
		inline Transform& GetTransform() { return mTransform; }

		void SetActorName(const std::string& arg_name) { mActorName = arg_name; };
		std::string GetActorName() const { return mActorName; };

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

		template<typename T>
		const std::vector<T*> GetComponents()
		{
			std::vector<T*> outVector;
			for (Component* comp : mComponents)
			{
				T* t = dynamic_cast<T*>(comp);
				if (t)
					outVector.push_back(t);
			}
			return outVector;
		}

		virtual void OnStart();
		virtual void OnTick(float arg_deltatime);

	};
}

#endif
