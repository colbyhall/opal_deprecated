// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector2.h"

SF_CORE_NAMESPACE_BEGIN

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

	SF_ALWAYS_INLINE constexpr Vector3() : x(0), y(0), z(0) {}
	SF_ALWAYS_INLINE constexpr Vector3(T xyz) : x(xyz), y(xyz), z(xyz) {}
	SF_ALWAYS_INLINE constexpr Vector3(T x, T y, T z) : u(x), v(y), w(z) {}
	SF_ALWAYS_INLINE constexpr Vector3(Vector2<T> xy, T z = 0);

	static const Vector3<T> zero;
	static const Vector3<T> one;
	static const Vector3<T> infinite;

	static const Vector3<T> forward;
	static const Vector3<T> right;
	static const Vector3<T> up;

	SF_ALWAYS_INLINE Vector3 operator+(const Vector3& rhs) const;
	SF_ALWAYS_INLINE Vector3 operator-(const Vector3& rhs) const;
	SF_ALWAYS_INLINE Vector3 operator*(const Vector3& rhs) const;
	SF_ALWAYS_INLINE Vector3 operator/(const Vector3& rhs) const;
	SF_ALWAYS_INLINE void operator+=(const Vector3& rhs) {
		*this = *this + rhs;
	}
	SF_ALWAYS_INLINE void operator-=(const Vector3& rhs) {
		*this = *this - rhs;
	}
	SF_ALWAYS_INLINE void operator*=(const Vector3& rhs) {
		*this = *this * rhs;
	}
	SF_ALWAYS_INLINE void operator/=(const Vector3& rhs) {
		*this = *this / rhs;
	}
	SF_ALWAYS_INLINE Vector3 operator-() const { return { -x, -y, -z }; }

	SF_ALWAYS_INLINE T dot(const Vector3& rhs) const;
	SF_ALWAYS_INLINE Vector3<T> cross(const Vector3& rhs) const;
	SF_ALWAYS_INLINE T len_sq() const { return dot(*this); }
	SF_ALWAYS_INLINE T len() const { return sf::sqrt(len_sq()); }
	SF_ALWAYS_INLINE Option<Vector3<T>> normalized() const;

	SF_NO_DISCARD SF_ALWAYS_INLINE bool is_zero() const {
		return *this == zero;
	}
	SF_NO_DISCARD SF_ALWAYS_INLINE bool is_near_zero() const;
	SF_NO_DISCARD SF_ALWAYS_INLINE bool has_nan() const;
	SF_NO_DISCARD SF_ALWAYS_INLINE bool has_infinite() const;

	template <typename D>
	SF_NO_DISCARD SF_ALWAYS_INLINE Vector3<D> cast() const {
		return { static_cast<D>(x), static_cast<D>(y), static_cast<D>(z) };
	}
};

SF_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/vector3.inl"

// Export to gj namespace
SF_NAMESPACE_BEGIN
using core::Vector3;
SF_NAMESPACE_END
