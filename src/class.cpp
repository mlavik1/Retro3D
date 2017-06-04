#include "class.h"
#include "object.h"
#include "st_assert.h"
#include "function.h"

namespace Retro3D
{

	Class::Class(const char* arg_name, staticconstructor_t constructor, Retro3D::Class* superclass)
		: mClassName(arg_name), mStaticConstructor(constructor), mBaseClass(superclass)
	{
		__AssertComment(arg_name != "", "ClassName cannot be empty");
		if (mBaseClass != nullptr)
		{
			mBaseClass->mChildClasses.push_back(this);
		}
		mCreatedInstanceCount = 0;
	}

	void Class::AddMemberFunction(Function* arg_function)
	{
		mMemberFunctions[arg_function->mFunctionName] = arg_function;
	}

	const std::string &Class::GetFullName() const
	{
		return mClassName;
	}


	std::string Class::GetName() const
	{
		size_t i = mClassName.find_last_of(':');
		if (i == std::string::npos)
		{
			return mClassName;
		}
		return mClassName.substr(i + 1, mClassName.size() - i - 1);
	}


	const Class* Class::GetBaseClass() const
	{
		return mBaseClass;

	}


	bool Class::IsSubclassOf(Class* arg_other) const
	{
		if (this == arg_other)
			return true;
		else if (mBaseClass == nullptr)
			return false;
		else
			return mBaseClass->IsSubclassOf(arg_other);
	}


	bool Class::IsA(Class* arg_other) const
	{
		return this == arg_other || this->IsSubclassOf(arg_other);
	}


	Object* Class::CreateInstance()
	{
		if (mStaticConstructor != nullptr)
		{
			Object* createdObject = mStaticConstructor();
			return createdObject;
		}
		else
		{
			return nullptr;
		}
	}


	Class* Class::GetChildClassByName(const char* arg_name, bool arg_fullname) const
	{
		for (Class* childClass : mChildClasses)
		{
			if ((arg_fullname && std::strcmp(childClass->GetFullName().c_str(), arg_name) == 0)
				|| (!arg_fullname && std::strcmp(childClass->GetName().c_str(), arg_name) == 0))
			{
				return childClass;
			}
			return childClass->GetChildClassByName(arg_name, arg_fullname);
		}
		return nullptr;
	}

	Function* Class::GetFunctionByName(const char* arg_name)
	{
		auto iter = mMemberFunctions.find(arg_name);
		if (iter != mMemberFunctions.end())
		{
			return iter->second;
		}
		return nullptr;
	}


	Class* Class::GetClassByName(const char* arg_name, bool arg_fullname)
	{
		return Object::GetStaticClass()->GetChildClassByName(arg_name, arg_fullname);
	}
}
