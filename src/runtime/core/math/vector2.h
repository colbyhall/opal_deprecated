// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/math/math.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
struct alignas(16) Vector2 {
	union {
		struct {
			T x, y;
		};
		struct {
			T width, height;
		};
		struct {
			T u, v;
		};
		T xy[2];
	};

	GJ_ALWAYS_INLINE constexpr Vector2() : x(0), y(0) {}
	GJ_ALWAYS_INLINE constexpr Vector2(T xy) : x(xy), y(xy) {}
	GJ_ALWAYS_INLINE constexpr Vector2(T x, T y) : u(x), v(y) {}

	GJ_ALWAYS_INLINE static Vector2<T> from_rad(T rad);

	static const Vector2<T> zero;
	static const Vector2<T> one;
	static const Vector2<T> infinite;

	static const Vector2<T> right;
	static const Vector2<T> up;

	GJ_ALWAYS_INLINE Vector2 operator+(const Vector2& rhs) const;
	GJ_ALWAYS_INLINE Vector2 operator-(const Vector2& rhs) const;
	GJ_ALWAYS_INLINE Vector2 operator*(const Vector2& rhs) const;
	GJ_ALWAYS_INLINE Vector2 operator/(const Vector2& rhs) const;
	GJ_ALWAYS_INLINE void operator+=(const Vector2& rhs) {
		*this = *this + rhs;
	}
	GJ_ALWAYS_INLINE void operator-=(const Vector2& rhs) {
		*this = *this - rhs;
	}
	GJ_ALWAYS_INLINE void operator*=(const Vector2& rhs) {
		*this = *this * rhs;
	}
	GJ_ALWAYS_INLINE void operator/=(const Vector2& rhs) {
		*this = *this / rhs;
	}
	GJ_ALWAYS_INLINE Vector2 operator-() const { return { -x, -y }; }

	GJ_ALWAYS_INLINE T dot(const Vector2<T>& rhs) const;
	GJ_ALWAYS_INLINE T cross(const Vector2<T>& rhs) const;
	GJ_ALWAYS_INLINE Vector2<T> perp() const;
	GJ_ALWAYS_INLINE T len_sq() const { return dot(*this); }
	GJ_ALWAYS_INLINE T len() const { return gj::sqrt(len_sq()); }
	GJ_ALWAYS_INLINE Option<Vector2<T>> normalized() const;

	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_zero() const {
		return *this == zero;
	}
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_near_zero() const;
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool has_nan() const;
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool has_infinite() const;

	template <typename D>
	GJ_NO_DISCARD GJ_ALWAYS_INLINE Vector2<D> cast() const {
		return { static_cast<D>(x), static_cast<D>(y) };
	}
};

GJ_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/vector2.inl"

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::Vector2;
GJ_NAMESPACE_END
