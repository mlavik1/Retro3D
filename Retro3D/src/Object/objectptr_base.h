#ifndef RETRO3D_OBJECTPTRBASE_H
#define RETRO3D_OBJECTPTRBASE_H

/*=============================================================================================
Base class for object pointers.

==============================================================================================*/
#include "objectrefhandle.h"
#include <cstddef>

namespace Retro3D
{
	class Object;

	template <class T>
	class ObjectPtrBase
	{
	protected:
		ObjectRefHandle* mRefHandle;

	public:
		inline Object* GetObjectSafe() const { return (mRefHandle != nullptr ? mRefHandle->GetObject() : nullptr); }

	public:
		virtual ~ObjectPtrBase() {}

		T* Get() const;
		T* operator->() const;
		bool operator==(const ObjectPtrBase<T>& arg_other) const;
		bool operator!=(const ObjectPtrBase<T>& arg_other) const;
		bool operator==(const T* arg_other) const;
		bool operator!=(const T* arg_other) const;

		virtual bool IsValid() const;

		const ObjectRefHandle* GetRefHandle() const { return mRefHandle; }

		struct HashByRefHandle
		{
			size_t operator()(const ObjectPtrBase<T>& objPtr) const
			{
				return (size_t)objPtr.GetRefHandle();
			}
		};

	};
}

#include "objectptr_base.cpp"

#endif
