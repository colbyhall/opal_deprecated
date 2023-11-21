// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

OP_SUPPRESS_WARNINGS_STD_BEGIN
#include <initializer_list>
OP_SUPPRESS_WARNINGS_STD_END

// Export to op namespace
OP_NAMESPACE_BEGIN

template <typename T>
using InitializerList = std::initializer_list<T>;

OP_NAMESPACE_END
