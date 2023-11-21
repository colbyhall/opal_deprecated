// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

// Use to avoid warnings when including stb files
#define OP_STB_SUPRESS_WARNINGS                                                                                        \
	OP_SUPPRESS_WARNINGS                                                                                               \
	OP_MSVC_SUPPRESS_WARNING(4242)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4244)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(5262)
