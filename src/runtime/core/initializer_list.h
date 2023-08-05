// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

GJ_SUPPRESS_WARNINGS_STD_BEGIN
#include <initializer_list>
GJ_SUPPRESS_WARNINGS_STD_END

// Export to gj namespace
GJ_NAMESPACE_BEGIN

template <typename T>
using InitializerList = std::initializer_list<T>;

GJ_NAMESPACE_END
