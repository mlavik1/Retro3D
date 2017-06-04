#ifndef RETRO3D_OBJECTREFHANDLE_H
#define RETRO3D_OBJECTREFHANDLE_H

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
		Object* mObject;
		unsigned int mRefCount;

		void maybeDelete();

	public:
		ObjectRefHandle(Object* arg_object);

		inline Object* GetObject() const { return mObject; };

		inline void SetObject(Object* arg_object) { mObject = arg_object; }
		void AddRef();
		void RemoveRef();
	};
}

#endif
