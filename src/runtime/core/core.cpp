// Copyright Colby Hall. All Rights Reserved.

#include "core.h"

#ifdef SF_PLATFORM_WINDOWS
SF_SUPPRESS_WARNING_PUSH
SF_MSVC_SUPPRESS_WARNING(5039) // 10.0.19041.0\um\winbase.h(7679)
SF_MSVC_SUPPRESS_WARNING(4668)
	#define UNICODE
	#define WIN32_MEAN_AND_LEAN
	#include <Windows.h>

	#undef min
	#undef max
SF_SUPPRESS_WARNING_POP

SF_SUPPRESS_WARNINGS_STD_BEGIN
	#include <any>
	#include <cstdio>
SF_SUPPRESS_WARNINGS_STD_END

	#ifdef SF_ENABLE_ASSERTS
SF_CORE_NAMESPACE_BEGIN

SF_CORE_NAMESPACE_END
	#endif

#else
	#error Unimplemented Assert
#endif
