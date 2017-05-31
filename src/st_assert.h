#ifndef RETRO3D_ASSERT_H
#define RETRO3D_ASSERT_H

#ifdef _WIN32
#define __DebugBreakImpl __debugbreak()
#else
#define __DebugBreakImpl
#endif

#include <assert.h>
#include "debug.h"
#ifdef _WIN32
#include <intrin.h>
#endif

#define __Assert(expr) \
	if(!(expr)) { \
		LOG_ERROR() << "Assertion error on " << #expr; \
		__DebugBreakImpl; \
	}

#define __AssertComment(expr, comment) \
	if(!(expr)) { \
		LOG_ERROR() << "Assertion error on " << #expr << ", \"" << comment << "\""; \
		__DebugBreakImpl; \
	}
	

#endif
