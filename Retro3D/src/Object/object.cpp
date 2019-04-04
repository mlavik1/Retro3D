#include "object.h"
#include "objectrefhandle.h"
#include <sstream>

IMPLEMENT_CLASS(Retro3D::Object)

namespace Retro3D
{
	Object::Object()
	{
		mObjectRefHandle = new ObjectRefHandle(this);

		std::stringstream ss;
		ss << GetClass()->GetName() << GetClass()->mCreatedInstanceCount;
		mObjectName = ss.str();

		GetClass()->mCreatedInstanceCount++;
	}

	Object::~Object()
	{
		mObjectRefHandle->SetObject(nullptr);
	}

	void Object::CallFunction(Function* arg_function, FunctionArgContainer args)
	{
		(this->*(arg_function->mFunctionPointer))(args);
	}

	void Object::InitialiseObject(ObjectInitialiserParams arg_params)
	{

	}
}
