#ifndef HIKARI_ASSERT_H
#define HIKARI_Assert_H

#include <assert.h>
#include "debug.h"
#include <intrin.h>

#define __Assert(expr) \
	if(!(expr)) { \
		LOG_ERROR() << "Assertion error on " << #expr; \
		__debugbreak(); \
	}

#define __AssertComment(expr, comment) \
	if(!(expr)) { \
		LOG_ERROR() << "Assertion error on " << #expr << ", \"" << comment << "\""; \
	__debugbreak(); \
	}
	

#endif
