// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <core/core.h>

EU_CORE_NAMESPACE_BEGIN

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

EU_CORE_NAMESPACE_END

// Export to of core namespace
EU_NAMESPACE_BEGIN
using core::NonCopyable;
EU_NAMESPACE_END
