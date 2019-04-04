#ifndef RETRO3D_OBJECTPTRBASE_CPP
#define RETRO3D_OBJECTPTRBASE_CPP

#include "objectptr_base.h"
#include "object.h"
#include <type_traits>

namespace Retro3D
{
	template<typename T>
	T* ObjectPtrBase<T>::Get() const
	{
		return static_cast<T*>(mRefHandle->GetObject());
	}

	template<typename T>
	T* ObjectPtrBase<T>::operator->() const
	{
		return static_cast<T*>(GetObjectSafe());
	}

	template<typename T>
	bool ObjectPtrBase<T>::operator==(const ObjectPtrBase<T>& arg_other) const
	{
		return GetObjectSafe() == arg_other.GetObjectSafe();
	}

	template<typename T>
	bool ObjectPtrBase<T>::operator!=(const ObjectPtrBase<T>& arg_other) const
	{
		return GetObjectSafe() != arg_other.GetObjectSafe();
	}

	template<typename T>
	bool ObjectPtrBase<T>::operator==(const T* arg_other) const
	{
		return GetObjectSafe() == arg_other;
	}

	template<typename T>
	bool ObjectPtrBase<T>::operator!=(const T* arg_other) const
	{
		return GetObjectSafe() != arg_other;
	}

	template<typename T>
	bool ObjectPtrBase<T>::IsValid() const
	{
		return GetObjectSafe() != nullptr;
	}

}

#endif
