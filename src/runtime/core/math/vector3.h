// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector2.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
struct alignas(16) Vector3 {
	union {
		struct {
			T x, y, z;
		};
		struct {
			T u, v, w;
		};
		Vector2<T> xy;
		T xyz[3];
	};

	GJ_ALWAYS_INLINE constexpr Vector3() : x(0), y(0), z(0) {}
	GJ_ALWAYS_INLINE constexpr Vector3(T xyz) : x(xyz), y(xyz), z(xyz) {}
	GJ_ALWAYS_INLINE constexpr Vector3(T x, T y, T z) : u(x), v(y), w(z) {}
	GJ_ALWAYS_INLINE constexpr Vector3(Vector2<T> xy, T z = 0);

	static const Vector3<T> zero;
	static const Vector3<T> one;
	static const Vector3<T> infinite;

	static const Vector3<T> forward;
	static const Vector3<T> right;
	static const Vector3<T> up;

	GJ_ALWAYS_INLINE Vector3 operator+(const Vector3& rhs) const;
	GJ_ALWAYS_INLINE Vector3 operator-(const Vector3& rhs) const;
	GJ_ALWAYS_INLINE Vector3 operator*(const Vector3& rhs) const;
	GJ_ALWAYS_INLINE Vector3 operator/(const Vector3& rhs) const;
	GJ_ALWAYS_INLINE void operator+=(const Vector3& rhs) {
		*this = *this + rhs;
	}
	GJ_ALWAYS_INLINE void operator-=(const Vector3& rhs) {
		*this = *this - rhs;
	}
	GJ_ALWAYS_INLINE void operator*=(const Vector3& rhs) {
		*this = *this * rhs;
	}
	GJ_ALWAYS_INLINE void operator/=(const Vector3& rhs) {
		*this = *this / rhs;
	}
	GJ_ALWAYS_INLINE Vector3 operator-() const { return { -x, -y, -z }; }

	GJ_ALWAYS_INLINE T dot(const Vector3& rhs) const;
	GJ_ALWAYS_INLINE Vector3<T> cross(const Vector3& rhs) const;
	GJ_ALWAYS_INLINE T len_sq() const { return dot(*this); }
	GJ_ALWAYS_INLINE T len() const { return gj::sqrt(len_sq()); }
	GJ_ALWAYS_INLINE Option<Vector3<T>> normalized() const;

	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_zero() const {
		return *this == zero;
	}
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_near_zero() const;
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool has_nan() const;
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool has_infinite() const;

	template <typename D>
	GJ_NO_DISCARD GJ_ALWAYS_INLINE Vector3<D> cast() const {
		return { static_cast<D>(x), static_cast<D>(y), static_cast<D>(z) };
	}
};

GJ_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/vector3.inl"

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::Vector3;
GJ_NAMESPACE_END
