#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "st_assert.h"

#define __DeclareSingleton(type) \
public: \
	static type *Instance() \
		{ \
			__Assert((_instance != 0)); \
			return _instance; \
		}; \
	static type *Create(); \
private: \
	static type *_instance;

#define __ImplementSingleton(type) \
	type *type::_instance = 0; \
	type *type::Create() \
		{ \
			type *newObject = new type(); \
			return newObject; \
		}

#define __ImplementTemplateSingleton(type) \
	template <typename T> \
	type *type::_instance = 0; \
	template <typename T> \
	type *type::Create() \
		{ \
			type *newObject = new type(); \
			return newObject; \
		}

#define __ConstructSingleton(type) \
    __Assert(_instance == 0); _instance = this;

#define __DestructSingleton(type) \
	__Assert(_instance); _instance = 0;

#endif