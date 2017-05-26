#include "object.h"
#include "objectrefhandle.h"

namespace Retro3D
{
	Object::Object()
	{
		mObjectRefHandle = new ObjectRefHandle(this);

	}

	Object::~Object()
	{
		mObjectRefHandle->SetObject(nullptr);
	}
}
