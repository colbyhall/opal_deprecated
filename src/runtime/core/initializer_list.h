// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

EU_SUPPRESS_WARNINGS_STD_BEGIN
#include <initializer_list>
EU_SUPPRESS_WARNINGS_STD_END

// Export to eu namespace
EU_NAMESPACE_BEGIN

template <typename T>
using InitializerList = std::initializer_list<T>;

EU_NAMESPACE_END
