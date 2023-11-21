// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

OP_SUPPRESS_WARNINGS_STD_BEGIN
#include <tuple>
OP_SUPPRESS_WARNINGS_STD_END

OP_CORE_NAMESPACE_BEGIN

template <typename T>
using Tuple = std::tuple<T>;

OP_CORE_NAMESPACE_END

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Tuple;
OP_NAMESPACE_END
