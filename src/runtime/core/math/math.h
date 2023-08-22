// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

SF_SUPPRESS_WARNINGS_STD_BEGIN
#include <limits>
SF_SUPPRESS_WARNINGS_STD_END

SF_CORE_NAMESPACE_BEGIN

template <typename T>
constexpr inline T min(T a, T b) {
	return a < b ? a : b;
}

template <typename T>
constexpr inline T max(T a, T b) {
	return a > b ? a : b;
}

template <typename T>
constexpr inline T clamp(T value, T min, T max) {
	return core::min<T>(core::max<T>(value, min), max);
}

template <typename T>
inline constexpr T pi;

template <>
inline constexpr f32 pi<f32> = 3.14159265359f;

template <>
inline constexpr f64 pi<f64> = 3.14159265359;

template <typename T>
inline constexpr T tau = pi<T> * (T)2;

template <typename T>
inline constexpr T deg_to_rad = pi<T> / (T)180;

template <typename T>
inline constexpr T rad_to_deg = (T)180 / pi<T>;

template <typename T>
inline constexpr T near_zero;

template <>
inline constexpr f32 near_zero<f32> = 1.0e-8f;

template <>
inline constexpr f64 near_zero<f64> = 1.0e-8;

template <typename T>
inline constexpr T infinity = std::numeric_limits<T>::infinity();

template <typename T>
inline constexpr T epsilon = std::numeric_limits<T>::epsilon();

template <typename T>
inline constexpr T nan = std::numeric_limits<T>::signaling_NaN();

SF_NO_DISCARD f32 cos(f32 x);
SF_NO_DISCARD f64 cos(f64 x);

SF_NO_DISCARD f32 sin(f32 x);
SF_NO_DISCARD f64 sin(f64 x);

SF_NO_DISCARD f32 tan(f32 x);
SF_NO_DISCARD f64 tan(f64 x);

SF_NO_DISCARD f32 acos(f32 x);
SF_NO_DISCARD f64 acos(f64 x);

SF_NO_DISCARD f32 asin(f32 x);
SF_NO_DISCARD f64 asin(f64 x);

SF_NO_DISCARD f32 atan(f32 x);
SF_NO_DISCARD f64 atan(f64 x);

SF_NO_DISCARD f32 atan2(f32 y, f32 x);
SF_NO_DISCARD f64 atan2(f64 y, f64 x);

SF_NO_DISCARD f32 sqrt(f32 x);
SF_NO_DISCARD f64 sqrt(f64 x);

SF_NO_DISCARD f32 fmod(f32 numerator, f32 denominator);
SF_NO_DISCARD f64 fmod(f64 numerator, f64 denominator);

template <typename T>
SF_ALWAYS_INLINE T abs(T x) {
	return x < 0 ? -x : x;
}

template <typename T>
SF_ALWAYS_INLINE bool is_near_zero(T a) {
	return abs(a) <= near_zero<T>;
}

template <typename T>
SF_ALWAYS_INLINE T lerp(T a, T b, T t) {
	return ((T)1 - t) * a + t * b;
}

SF_CORE_NAMESPACE_END

SF_NAMESPACE_BEGIN
using core::abs;
using core::acos;
using core::asin;
using core::atan;
using core::atan2;
using core::clamp;
using core::cos;
using core::fmod;
using core::lerp;
using core::max;
using core::min;
using core::sin;
using core::sqrt;
using core::tan;

template <typename T>
inline constexpr T pi = core::pi<T>;

template <typename T>
inline constexpr T tau = core::tau<T>;

template <typename T>
inline constexpr T deg_to_rad = core::deg_to_rad<T>;

template <typename T>
inline constexpr T rad_to_deg = core::rad_to_deg<T>;

template <typename T>
inline constexpr T infinity = core::infinity<T>;

template <typename T>
inline constexpr T epsilon = core::epsilon<T>;

template <typename T>
inline constexpr T nan = core::nan<T>;

SF_NAMESPACE_END
