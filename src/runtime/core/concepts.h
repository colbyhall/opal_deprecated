// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

OP_SUPPRESS_WARNINGS_STD_BEGIN
#include <concepts>
OP_SUPPRESS_WARNINGS_STD_END

OP_NAMESPACE_BEGIN

template <typename A, typename B>
concept SameAs = std::same_as<A, B>;

template <typename Derived, typename Base>
concept DerivedFrom = std::derived_from<Derived, Base>;

template <typename From, typename To>
concept ConvertibleTo = std::convertible_to<From, To>;

template <typename T>
concept Integral = std::integral<T>;

template <typename T>
concept SignedIntegral = std::signed_integral<T>;

template <typename T>
concept UnsignedIntegral = std::unsigned_integral<T>;

template <typename T>
concept FloatingPoint = std::floating_point<T>;

template <typename T>
concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

template <typename T>
concept Copyable = std::copyable<T>;

template <typename T>
concept Movable = std::movable<T>;

template <typename T>
concept Reference = std::is_reference_v<T>;

template <typename T>
concept Number = Integral<T> || FloatingPoint<T>;

OP_NAMESPACE_END
