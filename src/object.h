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

	protected:
		std::string mObjectName;

	public:
		Object();
		~Object();

		virtual void InitialiseObject(ObjectInitialiserParams arg_params);

		void CallFunction(Function* arg_function, FunctionArgContainer args);

		inline ObjectRefHandle*  GetRefHandle() { return mObjectRefHandle; }
	};
}

#endif
