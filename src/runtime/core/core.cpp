// Copyright Colby Hall. All Rights Reserved.

#include "core.h"

#ifdef EU_PLATFORM_WINDOWS
EU_SUPPRESS_WARNING_PUSH
EU_MSVC_SUPPRESS_WARNING(5039) // 10.0.19041.0\um\winbase.h(7679)
EU_MSVC_SUPPRESS_WARNING(4668)
	#define UNICODE
	#define WIN32_MEAN_AND_LEAN
	#include <Windows.h>

	#undef min
	#undef max
EU_SUPPRESS_WARNING_POP

EU_SUPPRESS_WARNINGS_STD_BEGIN
	#include <any>
	#include <cstdio>
EU_SUPPRESS_WARNINGS_STD_END

	#ifdef EU_ENABLE_ASSERTS
EU_CORE_NAMESPACE_BEGIN

auto _assert_failed(bool must_crash, const char* expression,
					const char* message, const char* file, u32 line) -> bool {
	char buffer[1024];
	buffer[0] = 0;

	static const char* ensure_usage =
		"Press Yes to breakpoint. No to continue.";
	if (message != nullptr) {
		if (must_crash) {
			sprintf_s(buffer, "%s\n\n(%s) evaluated to false\n\n%s Line %d",
					  message, expression, file, line);
		} else {
			sprintf_s(buffer,
					  "%s\n\n(%s) evaluated to false\n\n%s Line %d\n\n%s",
					  message, expression, file, line, ensure_usage);
		}
	} else {
		if (must_crash) {
			sprintf_s(buffer, "(%s) evaluated to false\n\n%s Line %d",
					  expression, file, line);
		} else {
			sprintf_s(buffer, "(%s) evaluated to false\n\n%s Line %d\n\n%s",
					  expression, file, line, ensure_usage);
		}
	}
	const char* title =
		must_crash ? "Application Assert" : "Application Ensure";
	const int result =
		MessageBoxA(NULL, buffer, title,
					UINT(must_crash ? (MB_OK | MB_ICONERROR)
									: (MB_YESNO | MB_ICONWARNING)));
	return result != IDNO;
}

EU_CORE_NAMESPACE_END
	#endif

#else
	#error Unimplemented Assert
#endif
