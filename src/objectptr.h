#ifndef RETRO3D_OBJECTPTR_H
#define RETRO3D_OBJECTPTR_H

/*=============================================================================================
Reference-counted pointer to a Retro3D::Object.
When the object is destroyed (in any way), this will return nullptr.

==============================================================================================*/
#include "objectrefhandle.h"

namespace Retro3D
{
	class Object;

	template <class T>
	class ObjectPtr
	{
	private:
		ObjectRefHandle* mRefHandle;

		inline Object* getObjectSafe() const { return (mRefHandle != nullptr ? mRefHandle->GetObject() : nullptr); }

	public:
		ObjectPtr();
		ObjectPtr(Object* arg_object);
		~ObjectPtr();

		ObjectPtr<T>& operator=(const ObjectPtr<T>& arg_other);

		T* Get();
		T* operator->() const;
		bool operator==(const ObjectPtr<T>& arg_other) const;
		bool operator!=(const ObjectPtr<T>& arg_other) const;
		bool operator==(const T* arg_other) const;
		bool operator!=(const T* arg_other) const;

		bool IsValid() const;

	};
}

#include "objectptr.cpp"

#endif
