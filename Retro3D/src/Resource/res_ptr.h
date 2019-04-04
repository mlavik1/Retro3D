#ifndef RETRO3D_RESPTR_H
#define RETRO3D_RESPTR_H

#include "Object/objectptr.h"
#include "resource.h"

namespace Retro3D
{
	template <class T>
	class ResPtr : public ObjectPtr<T>
	{
	public:
		ResPtr();
		ResPtr(Resource* arg_object);
		virtual ~ResPtr() override;
		virtual bool IsValid() const override;

	};
}

#include "res_ptr.cpp"

#endif
