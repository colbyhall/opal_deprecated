// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

SF_SUPPRESS_WARNING_PUSH
SF_MSVC_SUPPRESS_WARNING(5039) // 10.0.19041.0\um\winbase.h(7679)
SF_MSVC_SUPPRESS_WARNING(4668)

#define UNICODE
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

#undef min
#undef max

SF_SUPPRESS_WARNING_POP
