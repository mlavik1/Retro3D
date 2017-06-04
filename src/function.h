#ifndef RETRO3D_FUNCTION_H
#define RETRO3D_FUNCTION_H

#include "object.h"
#include <string>

namespace Retro3D
{
	class FunctionArgContainer
	{
	public:
		char* mData;
		size_t mSize;

		FunctionArgContainer()
			: mData(nullptr)
		{
		}

		FunctionArgContainer(const FunctionArgContainer& arg_other)
		{
			mSize = arg_other.mSize;
			if (arg_other.mData != nullptr)
			{
				mData = new char[mSize];
				memcpy(mData, arg_other.mData, mSize);
			}
		}

		~FunctionArgContainer()
		{
			if (mData)
				delete mData;
		}

		template <typename T1>
		void TemplateConstruct(T1 t1)
		{
			mSize = sizeof(T1);
			mData = new char[mSize];
			*((T1*)mData) = t1;
		}

		template <typename T1, typename T2>
		void TemplateConstruct(T1 t1, T2 t2)
		{
			mSize = sizeof(T1) + sizeof(T2);
			mData = new char[mSize];
			*((T1*)mData) = t1;
			*((T2*)(mData + sizeof(T1))) = t2;
		}

		template <typename T1, typename T2, typename T3>
		void TemplateConstruct(T1 t1, T2 t2, T3 t3)
		{
			mSize = sizeof(T1) + sizeof(T2) + sizeof(T3);
			mData = new char[mSize];
			*((T1*)mData) = t1;
			*((T2*)(mData + sizeof(T1))) = t2;
			*((T3*)(mData + sizeof(T1) + sizeof(T2))) = t3;
		}


		template <typename T1>
		void TemplateAccess(T1 &t1)
		{
			t1 = *((T1*)mData);
		}

		template <typename T1, typename T2>
		void TemplateAccess(T1 &t1, T2 &t2)
		{
			t1 = *((T1*)mData);
			t2 = *((T2*)(mData + sizeof(T1)));
		}

		template <typename T1, typename T2, typename T3>
		void TemplateAccess(T1 &t1, T2 &t2, T3 &t3)
		{
			t1 = *((T1*)mData);
			t2 = *((T2*)(mData + sizeof(T1)));
			t3 = *((T3*)(mData + sizeof(T1) + sizeof(T2)));
		}

	};

	class Function
	{
	public:
		std::string mFunctionName;
		void(Retro3D::Object::*mFunctionPointer)(FunctionArgContainer);

	public:
		Function(std::string arg_name, void(Retro3D::Object::*arg_functionptr)(FunctionArgContainer))
		{
			mFunctionName = arg_name;
			mFunctionPointer = arg_functionptr;
		}
	
	};
}

#endif
