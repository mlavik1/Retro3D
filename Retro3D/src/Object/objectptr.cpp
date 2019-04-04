#ifndef RETRO3D_OBJECTPTR_CPP
#define RETRO3D_OBJECTPTR_CPP

#include "objectptr.h"
#include "Misc/st_assert.h"
#include "object.h"
#include <type_traits>

namespace Retro3D
{
	template<typename T>
	ObjectPtr<T>::ObjectPtr()
	{
		this->mRefHandle = nullptr;
		//__AssertComment((std::is_base_of<Object, T>::value), "Type of T must be Object or subclass of Object.");
	}

	template<typename T>
	ObjectPtr<T>::ObjectPtr(Object* arg_object)
	{
		__AssertComment((std::is_base_of<Object, T>::value), "Type of T must be Object or subclass of Object.");

		if (arg_object != nullptr)
		{
			this->mRefHandle = arg_object->GetRefHandle();
			this->mRefHandle->AddStrongRef();
		}
		else
		{
			this->mRefHandle = nullptr;
		}
	}

	template<typename T>
	ObjectPtr<T>::ObjectPtr(const ObjectPtr<T>& arg_other)
	{
		__AssertComment((std::is_base_of<Object, T>::value), "Type of T must be Object or subclass of Object.");

		if (arg_other != nullptr)
		{
			this->mRefHandle = arg_other.mRefHandle;
			this->mRefHandle->AddStrongRef();
		}
		else
		{
			this->mRefHandle = nullptr;
		}
	}

	template<typename T>
	ObjectPtr<T>::~ObjectPtr()
	{
		if (this->mRefHandle != nullptr)
		{
			this->mRefHandle->RemoveStrongRef();
		}
	}

	template<typename T>
	ObjectPtr<T>& ObjectPtr<T>::operator=(const ObjectPtr<T>& arg_other)
	{
		ObjectRefHandle* oldHandle = this->mRefHandle;
		this->mRefHandle = arg_other.mRefHandle;
		if(this->mRefHandle != nullptr)
			this->mRefHandle->AddStrongRef();
		if (oldHandle != nullptr)
			oldHandle->RemoveStrongRef();
		return (*this);
	}

	template<typename T>
	T* ObjectPtr<T>::Get() const
	{
		return static_cast<T*>(this->mRefHandle->GetObject());
	}

	template<typename T>
	T* ObjectPtr<T>::operator->() const
	{
		return static_cast<T*>(this->GetObjectSafe());
	}

	template<typename T>
	bool ObjectPtr<T>::operator==(const ObjectPtrBase<T>& arg_other) const
	{
		return this->GetObjectSafe() == arg_other.GetObjectSafe();
	}

	template<typename T>
	bool ObjectPtr<T>::operator!=(const ObjectPtrBase<T>& arg_other) const
	{
		return this->GetObjectSafe() != arg_other.GetObjectSafe();
	}

	template<typename T>
	bool ObjectPtr<T>::operator==(const T* arg_other) const
	{
		return this->GetObjectSafe() == arg_other;
	}

	template<typename T>
	bool ObjectPtr<T>::operator!=(const T* arg_other) const
	{
		return this->GetObjectSafe() != arg_other;
	}

	template<typename T>
	bool ObjectPtr<T>::IsValid() const
	{
		return this->GetObjectSafe() != nullptr;
	}

}

#endif
