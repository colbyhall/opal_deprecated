// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/non_copyable.h"

OP_CORE_NAMESPACE_BEGIN

class Interface : NonCopyable {
public:
	virtual ~Interface() {}
};

OP_CORE_NAMESPACE_END

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Interface;
OP_NAMESPACE_END
