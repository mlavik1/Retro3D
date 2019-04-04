#ifndef RETRO3D_CLASS_H
#define RETRO3D_CLASS_H

/*=============================================================================================
							A Class structure for Retro3D::Object

Contains RunTime Type Information and some basic reflection-like functionalities.
Use DEFINE_CLASS and IMPLEMENT_CLASS, and inherit from Retro3D::Object to generate a Retro3D::Class.
See objdefs.h
==============================================================================================*/

#include <string>
#include <vector>
#include <unordered_map>

namespace Retro3D
{
	// Forward declarations:
	class Object;
	class Function;

	class Class
	{
		friend class Object;

		typedef Retro3D::Class*(*staticclassaccessor_t)();
		typedef Retro3D::Object*(*staticconstructor_t)();

	private:
		/** Name of the class */
		std::string mClassName;

		/** Pointer to base class */
		Retro3D::Class* mBaseClass;

		/** List of child classes of this class (specified in DEFINE_CLASS-macro) */
		std::vector<Retro3D::Class*> mChildClasses;

		/** Static functions that calls an empty constructor */
		staticconstructor_t mStaticConstructor;

		/** Number of created instances (destroyed isntances are also counted) */
		unsigned int mCreatedInstanceCount;

		/** Pointers to member functions of the class (registered with REGISTER_CLASS_FUNCTION-macro) */
		std::unordered_map<std::string, Function*> mMemberFunctions;

	public:
		Class(const char* arg_name, staticconstructor_t constructor = 0, Retro3D::Class* superclass = 0);

		void AddMemberFunction(Function* arg_function);

		/**
		* Gets the full name of the class, with namespace.
		*
		*/
		const std::string &GetFullName() const;

		/**
		* Gets the simple name of the class, without namespace.
		*
		*/
		std::string GetName() const;

		/**
		* Returns a pointer to the base class of this class.
		*
		*/
		const Class* GetBaseClass() const;

		/**
		* Checks if this class is an subclass of the specified class.
		*
		*/
		bool IsSubclassOf(Class* arg_other) const;

		/**
		* Checks if this class is the same as, or inherits from, the specified class.
		*
		*/
		bool IsA(Class* arg_other) const;

		/**
		* Creates an instances of the class.
		*
		*/
		Object* CreateInstance();
		
		/**
		* Recursively searches through the children searching for class by given name.
		*
		* @param arg_name				The name of the class to search for.
		* @param arg_fullname			Use full name (with namespace) when comparing class names.
		*/
		Class* GetChildClassByName(const char* arg_name, bool arg_fullname) const;

		Function* GetFunctionByName(const char* arg_name);

		/**
		* Returns a pointer to the static Class-instance of a class by the given name (if it exists).
		*
		* @param arg_name				The name of the class to search for.
		* @param arg_fullname			Use full name (with namespace) when comparing class names.
		*/
		static Class* GetClassByName(const char* arg_name, bool arg_fullname);

	};
}

#endif
