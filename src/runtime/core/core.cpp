// Copyright Colby Hall. All Rights Reserved.

#include "core.h"

#ifdef GJ_PLATFORM_WINDOWS
GJ_SUPPRESS_WARNING_PUSH
GJ_MSVC_SUPPRESS_WARNING(5039) // 10.0.19041.0\um\winbase.h(7679)
GJ_MSVC_SUPPRESS_WARNING(4668)
	#define UNICODE
	#define WIN32_MEAN_AND_LEAN
	#include <Windows.h>

	#undef min
	#undef max
GJ_SUPPRESS_WARNING_POP

GJ_SUPPRESS_WARNINGS_STD_BEGIN
	#include <any>
	#include <cstdio>
GJ_SUPPRESS_WARNINGS_STD_END

	#ifdef GJ_ENABLE_ASSERTS
GJ_CORE_NAMESPACE_BEGIN

GJ_CORE_NAMESPACE_END
	#endif

#else
	#error Unimplemented Assert
#endif
