// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

SF_SUPPRESS_WARNINGS_STD_BEGIN
#include <tuple>
SF_SUPPRESS_WARNINGS_STD_END

SF_CORE_NAMESPACE_BEGIN

template <typename T>
using Tuple = std::tuple<T>;

SF_CORE_NAMESPACE_END

// Export to sf namespace
SF_NAMESPACE_BEGIN
using core::Tuple;
SF_NAMESPACE_END
