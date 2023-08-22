// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

SF_SUPPRESS_WARNINGS_STD_BEGIN
#include <initializer_list>
SF_SUPPRESS_WARNINGS_STD_END

// Export to gj namespace
SF_NAMESPACE_BEGIN

template <typename T>
using InitializerList = std::initializer_list<T>;

SF_NAMESPACE_END
