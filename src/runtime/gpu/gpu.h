// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/shared.h"

// Begin the gpu namespace
#define EU_GPU_NAMESPACE_BEGIN                                                                                         \
	EU_NAMESPACE_BEGIN                                                                                                 \
	namespace gpu {

// End the gpu namespace
#define EU_GPU_NAMESPACE_END                                                                                           \
	}                                                                                                                  \
	EU_NAMESPACE_END

#define EU_GPU_DEBUG 1

EU_GPU_NAMESPACE_BEGIN
template <typename T>
using Shared = AtomicShared<T>;
EU_GPU_NAMESPACE_END