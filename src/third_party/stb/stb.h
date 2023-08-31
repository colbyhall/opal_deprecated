// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

// Use to avoid warnings when including stb files
#define SF_STB_SUPRESS_WARNINGS                                                                                        \
	SF_SUPPRESS_WARNINGS                                                                                               \
	SF_MSVC_SUPPRESS_WARNING(4242)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4244)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(5262)
