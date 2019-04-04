#ifndef RETRO3D_OBJDEFS_H
#define RETRO3D_OBJDEFS_H

/*=============================================================================================
Macros and more for Retro3D::Object and subclasses.

Enabling reflection on a class:
	class YourClass : public Object
	{
	DEFINE_CLASS(YourClass, Object)
	...
	}

Registering a function (so you can call it by name or use RPCs):
	DEFINE_FUNCTION(FunctionName, FunctionParameter1Type, FunctionParameter2Type, ......)

	BEGIN_REGISTER_CLASSPROPERTIES(Retro3D::YourClass)
	REGISTER_CLASS_FUNCTION(Retro3D::YourClass, FunctionName) // REPEAT FOR EACH FUNCTION
	.....
	END_REGISTER_CLASSPROPERTIES(Retro3D::YourClass)

==============================================================================================*/

#include "Misc/macros.h"
#include "class.h"
#include "function.h"

typedef unsigned int ObjectFlagRegister;

enum class ObjectFlag
{
	Destroyed = 1,
	PendingDestroy = 2,
	AsyncLoading = 4
};

typedef uint32_t NetGUID;

// Used in the constructor defined in DEFINE_CLASS_INTERNAL.
// This allows us to create an instance of a class without knowing the calss ( see Class::CreateInstance() ).
struct Retro3DObject_EmptyConstructorParams
{
	
};

struct ObjectInitialiserParams
{

};

// Do not use this directly!
#define DEFINE_CLASS_INTERNAL(name, constructorinitlist, baseclassaccessor) \
private: \
	static Retro3D::Class* StaticClass; \
public: \
	name (Retro3DObject_EmptyConstructorParams params) constructorinitlist  \
	{ \
	} \
	static Retro3D::Object* _CreateInstanceFromDefaultConstructor() \
	{ \
		return new name (Retro3DObject_EmptyConstructorParams()); \
	} \
	static Retro3D::Class* CreateStaticClass() \
	{ \
		if(StaticClass) return StaticClass; \
		return new Retro3D::Class(#name, & name ::_CreateInstanceFromDefaultConstructor, baseclassaccessor); \
	} \
	static Retro3D::Class* GetStaticClass() \
	{ \
		if(StaticClass == nullptr) StaticClass = name ::CreateStaticClass(); \
		return StaticClass; \
	} \
	virtual Retro3D::Class* GetClass() \
	{ \
		return GetStaticClass(); \
	}

/**
* Define a class, that directly or indirectly inherits from Retro3D::Object.
*
* The class will have Run-Time Type Information available through GetClass() and GetStaticClass().
* This macro will generate a constructor with parameters, so the class will have no implicit default constructor.
* IMPORTANT: You will need to add the IMPLEMENT_CLASS(classname)-macro to your source file.
*/
#define DEFINE_CLASS(classname, baseclassname) \
	DEFINE_CLASS_INTERNAL(classname, : baseclassname (params), baseclassname ::GetStaticClass())

/**
* Add this to your source file, if DEFINE_CLASS has been used in the header file.
*/
#define IMPLEMENT_CLASS(classname) \
	Retro3D::Class* classname ::StaticClass = classname ::CreateStaticClass();

#endif

#define BEGIN_REGISTER_CLASSPROPERTIES(classname) \
	static bool RegisterClassProperties() \
	{ \
		Retro3D::Class* myClass = ##classname##::GetStaticClass(); \

#define END_REGISTER_CLASSPROPERTIES(classname) \
	return 0; \
	}

#define REGISTER_CLASS_FUNCTION(classname, functionname) \
	myClass->AddMemberFunction(new Retro3D::Function(#functionname, (void(Retro3D::Object::*)(Retro3D::FunctionArgContainer))(&##classname##::call_##functionname##) ));

#define REGISTER_CLASSPROPERTIES(classname) \
	bool sdfsdfjkjghrghre = ##classname##::RegisterClassProperties();
	
#define CALL_FUNCTION(objectptr, functionname, ...) \
	objectptr->CallFunction(objectptr->GetClass()->GetFunctionByName(#functionname), getargs_##functionname##(__VA_ARGS__));

/**
* Define a function.
* This will enable RTTI and simple Reflection - and is required for RPC calls.
*/
#define DEFINE_FUNCTION(functionname, ...) \
FunctionArgContainer BOOST_PP_CAT(getargs_, functionname) BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__),(),GetDargs(Bogus, __VA_ARGS__)) \
{ \
	FunctionArgContainer args; \
	BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__), ,args.TemplateConstruct<) __VA_ARGS__ BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__), ,>) BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__), ,GetCargs(Bogus, __VA_ARGS__);) \
	return args; \
} \
void BOOST_PP_CAT(call_, functionname) (FunctionArgContainer args) \
{ \
	BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__), , \
		MakeVars(Bogus,__VA_ARGS__) )\
	BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__), ,args.TemplateAccess<) __VA_ARGS__ BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__), ,>) BOOST_PP_IF(BOOST_PP_IS_EMPTY(__VA_ARGS__), ,GetCargs(Bogus, __VA_ARGS__);) \
	functionname GetCargs(Bogus, __VA_ARGS__); \
}
