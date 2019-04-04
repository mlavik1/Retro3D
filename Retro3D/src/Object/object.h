#ifndef RETRO3D_OBJECT_H
#define RETRO3D_OBJECT_H

/*=============================================================================================
Base class for most classes in the Engine.
Making a class inherit from Retro3D::Object will:
- Enable additional runtime type information + some basic "reflection" functionality.
- Allow you to use Retro3D::ObjectPtr<T> on instances of the class


==============================================================================================*/
#include "objdefs.h"
#include <string>

namespace Retro3D
{
	class ObjectRefHandle;

	class Object
	{
		DEFINE_CLASS_INTERNAL(Object, , 0)

	private:
		ObjectRefHandle* mObjectRefHandle;
		ObjectFlagRegister mObjectFlags;

	protected:
		std::string mObjectName;

	public:
		Object();
		virtual ~Object();

		virtual void InitialiseObject(ObjectInitialiserParams arg_params);

		/**
		* Calls a function by the given name.
		* Note, the function must have been registered using the REGISTER_CLASS_FUNCTION macro
		*/
		void CallFunction(Function* arg_function, FunctionArgContainer args);

		/**
		* Gets the ObjectRefHandle of the Object.
		* Smart pointers reference this handle instead of referencing the object directly.
		*/
		inline ObjectRefHandle*  GetRefHandle() { return mObjectRefHandle; }

		inline bool HasObjectFlag(ObjectFlag arg_flag) { return ((mObjectFlags & (ObjectFlagRegister)arg_flag)) != 0; }
		inline void SetObjectFlag(ObjectFlag arg_flag) { mObjectFlags |= (ObjectFlagRegister)arg_flag; }
		inline void ClearObjectFlag(ObjectFlag arg_flag) { mObjectFlags &= ~(ObjectFlagRegister)arg_flag; }

	};
}

#endif
