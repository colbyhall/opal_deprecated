// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

// Begin the gpu namespace
#define EU_GPU_NAMESPACE_BEGIN                                                 \
	EU_NAMESPACE_BEGIN                                                         \
	namespace gpu {

// End the gpu namespace
#define EU_GPU_NAMESPACE_END                                                   \
	}                                                                          \
	EU_NAMESPACE_END

#define EU_GPU_DEBUG 1