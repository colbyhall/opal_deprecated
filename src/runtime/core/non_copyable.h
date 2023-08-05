// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

GJ_CORE_NAMESPACE_BEGIN

/// Class that makes another class non-copyable.
///
/// Usage: Inherit from NonCopyable.
class NonCopyable {
public:
	NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
	NonCopyable(NonCopyable&&) noexcept {}
	void operator=(NonCopyable&&) noexcept {}
};

GJ_CORE_NAMESPACE_END

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::NonCopyable;
GJ_NAMESPACE_END
