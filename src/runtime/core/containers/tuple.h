// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

EU_SUPPRESS_WARNINGS_STD_BEGIN
#include <tuple>
EU_SUPPRESS_WARNINGS_STD_END

EU_CORE_NAMESPACE_BEGIN

template <typename T>
using Tuple = std::tuple<T>;

EU_CORE_NAMESPACE_END

// Export to eu namespace
EU_NAMESPACE_BEGIN
using core::Tuple;
EU_NAMESPACE_END
