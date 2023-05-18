// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

EU_SUPPRESS_WARNINGS_STD_BEGIN
#include <type_traits>
EU_SUPPRESS_WARNINGS_STD_END

EU_CORE_NAMESPACE_BEGIN

template <typename T>
inline constexpr bool is_trivially_copyable = std::is_trivially_copyable_v<T>;

template <bool Test, class T = void>
using EnabledIf = std::enable_if_t<Test, T>;

template <typename Base, typename Derived>
inline constexpr bool is_base_of = std::is_base_of_v<Base, Derived>;

EU_CORE_NAMESPACE_END
