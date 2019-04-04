#ifndef RETRO3D_ACTOR_H
#define RETRO3D_ACTOR_H

#include "Object/object.h"
#include "Math/transform.h"
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <string>
#include "Component/component.h"
#include "Object/objectptr.h"

namespace Retro3D
{

	class Actor : public Object
	{
	protected:
		Transform mTransform;
		std::vector<ObjectPtr<Component>> mComponents; // TODO: RTTI

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
			for (ObjectPtr<Component> comp : mComponents)
			{
				T* t = dynamic_cast<T*>(comp.Get());
				if (t)
					return t;
			}
		}

		template<typename T>
		const std::vector<T*> GetComponents()
		{
			std::vector<T*> outVector;
			for (ObjectPtr<Component> comp : mComponents)
			{
				T* t = dynamic_cast<T*>(comp.Get());
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
