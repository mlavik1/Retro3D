#ifndef RETRO3D_OBJECTREFHANDLE_H
#define RETRO3D_OBJECTREFHANDLE_H

#include <vector>

/*=============================================================================================
A reference handle that contains:
- raw pointer to a Retro3D::Object
- reference count

ObjectRefHandle should only belong to, and be instansiated by, a Retro3D::Object.
When the Retro3D::Object is destroyed, the object pointer of the handle will be sett to nullptr.
When the reference count is 0 and the object is destroyed, the ObjectRefHandle will be destroyed.

==============================================================================================*/
namespace Retro3D
{
	class Object;

	class ObjectRefHandle
	{
	private:
		/** The owning object */
		Object* mObject;

		/** Strong references (used by ObjectPtr) */
		unsigned int mStrongRefCount;

		/** Weak references (used by WeakObjectPtr) */
		unsigned int mWeakRefCount;

		void maybeDelete();

	public:
		ObjectRefHandle(Object* arg_object);

		/** Gets the owning object of the handle */
		inline Object* GetObject() const { return mObject; };

		/** Sets the owning object of the handle. */
		void SetObject(Object* arg_object);

		/** Adds a strong reference to the object. */
		void AddStrongRef();

		/** Removes a strong reference to the object. This might result in object destruction. */
		void RemoveStrongRef();

		/** Adds a weak reference to the object. Used by WeakObjectPtr. */
		void AddWeakRef();

		/** Removes a weak reference to the object. Used by WeakObjectPtr. */
		void RemoveWeakRef();

		// HACK! TTODO: Find a nicer way of doing this
		static std::vector<ObjectRefHandle*> PendingDeleteHandles;
	};
}

#endif
