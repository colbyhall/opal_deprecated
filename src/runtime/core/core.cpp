// Copyright Colby Hall. All Rights Reserved.

#include "core.h"

#ifdef OP_PLATFORM_WINDOWS
OP_SUPPRESS_WARNING_PUSH
OP_MSVC_SUPPRESS_WARNING(5039) // 10.0.19041.0\um\winbase.h(7679)
OP_MSVC_SUPPRESS_WARNING(4668)
	#define UNICODE
	#define WIN32_MEAN_AND_LEAN
	#include <Windows.h>

	#undef min
	#undef max
OP_SUPPRESS_WARNING_POP

OP_SUPPRESS_WARNINGS_STD_BEGIN
	#include <any>
	#include <cstdio>
OP_SUPPRESS_WARNINGS_STD_END

	#ifdef OP_ENABLE_ASSERTS
OP_CORE_NAMESPACE_BEGIN

OP_CORE_NAMESPACE_END
	#endif

#else
	#error Unimplemented Assert
#endif
