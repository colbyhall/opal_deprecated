// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

GJ_SUPPRESS_WARNINGS_STD_BEGIN
#include <tuple>
GJ_SUPPRESS_WARNINGS_STD_END

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
using Tuple = std::tuple<T>;

GJ_CORE_NAMESPACE_END

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::Tuple;
GJ_NAMESPACE_END
