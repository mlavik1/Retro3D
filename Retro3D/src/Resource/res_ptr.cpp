#ifndef RETRO3D_RESPTR_CPP
#define RETRO3D_RESPTR_CPP

#include "res_ptr.h"
#include "Object/objdefs.h"

namespace Retro3D
{
	template<typename T>
	ResPtr<T>::ResPtr()
	{

	}

	template<typename T>
	ResPtr<T>::ResPtr(Resource* arg_object)
		: ObjectPtr<T>::ObjectPtr(arg_object)
	{

	}

	template<typename T>
	ResPtr<T>::~ResPtr()
	{

	}

	template<typename T>
	bool ResPtr<T>::IsValid() const
	{
		return ObjectPtr<T>::IsValid() && !ObjectPtr<T>::Get()->HasObjectFlag(ObjectFlag::AsyncLoading);
	}
}

#endif
