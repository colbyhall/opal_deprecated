// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/shared.h"

// Begin the gpu namespace
#define GJ_GPU_NAMESPACE_BEGIN                                                 \
	GJ_NAMESPACE_BEGIN                                                         \
	namespace gpu {

// End the gpu namespace
#define GJ_GPU_NAMESPACE_END                                                   \
	}                                                                          \
	GJ_NAMESPACE_END

#define GJ_GPU_DEBUG 1

GJ_GPU_NAMESPACE_BEGIN
template <typename T>
using Shared = AtomicShared<T>;
GJ_GPU_NAMESPACE_END