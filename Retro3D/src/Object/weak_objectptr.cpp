#ifndef RETRO3D_WEAKOBJECTPTR_CPP
#define RETRO3D_WEAKOBJECTPTR_CPP

#include "weak_objectptr.h"
#include "Misc/st_assert.h"
#include "object.h"
#include <type_traits>

namespace Retro3D
{
	template<typename T>
	WeakObjectPtr<T>::WeakObjectPtr()
	{
		this->mRefHandle = nullptr;
		//__AssertComment((std::is_base_of<Object, T>::value), "Type of T must be Object or subclass of Object.");
	}

	template<typename T>
	WeakObjectPtr<T>::WeakObjectPtr(Object* arg_object)
	{
		__AssertComment((std::is_base_of<Object, T>::value), "Type of T must be Object or subclass of Object.");

		if (arg_object != nullptr)
		{
			this->mRefHandle = arg_object->GetRefHandle();
			this->mRefHandle->AddWeakRef();
		}
		else
		{
			this->mRefHandle = nullptr;
		}
	}

	template<typename T>
	WeakObjectPtr<T>::WeakObjectPtr(const WeakObjectPtr<T>& arg_other)
	{
		__AssertComment((std::is_base_of<Object, T>::value), "Type of T must be Object or subclass of Object.");

		if (arg_other != nullptr)
		{
			this->mRefHandle = arg_other.mRefHandle;
			this->mRefHandle->AddWeakRef();
		}
		else
		{
			this->mRefHandle = nullptr;
		}
	}

	template<typename T>
	WeakObjectPtr<T>::WeakObjectPtr(const ObjectPtrBase<T>& arg_other)
		:WeakObjectPtr<T>::WeakObjectPtr(arg_other.GetObjectSafe())
	{
	}

	template<typename T>
	WeakObjectPtr<T>::~WeakObjectPtr()
	{
		if (this->mRefHandle != nullptr)
		{
			this->mRefHandle->RemoveWeakRef();
		}
	}

	template<typename T>
	WeakObjectPtr<T>& WeakObjectPtr<T>::operator=(const WeakObjectPtr<T>& arg_other)
	{
		ObjectRefHandle* oldHandle = this->mRefHandle;
		this->mRefHandle = arg_other.mRefHandle;
		if (this->mRefHandle != nullptr)
			this->mRefHandle->AddWeakRef();
		if (oldHandle != nullptr)
			oldHandle->RemoveWeakRef();
		return (*this);
	}

	template<typename T>
	T* WeakObjectPtr<T>::Get() const
	{
		return static_cast<T*>(this->mRefHandle->GetObject());
	}

	template<typename T>
	T* WeakObjectPtr<T>::operator->() const
	{
		return static_cast<T*>(this->GetObjectSafe());
	}

	template<typename T>
	bool WeakObjectPtr<T>::operator==(const ObjectPtrBase<T>& arg_other) const
	{
		return this->GetObjectSafe() == arg_other.GetObjectSafe();
	}

	template<typename T>
	bool WeakObjectPtr<T>::operator!=(const ObjectPtrBase<T>& arg_other) const
	{
		return this->GetObjectSafe() != arg_other.GetObjectSafe();
	}

	template<typename T>
	bool WeakObjectPtr<T>::operator==(const T* arg_other) const
	{
		return this->GetObjectSafe() == arg_other;
	}

	template<typename T>
	bool WeakObjectPtr<T>::operator!=(const T* arg_other) const
	{
		return this->GetObjectSafe() != arg_other;
	}

	template<typename T>
	bool WeakObjectPtr<T>::IsValid() const
	{
		return this->GetObjectSafe() != nullptr;
	}

}

#endif
