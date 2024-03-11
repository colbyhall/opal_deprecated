// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector3.h"

OP_CORE_NAMESPACE_BEGIN

template <Number T>
struct alignas(16) Vector4 {
	union {
		struct {
			T x, y, z, w;
		};
		struct {
			T r, g, b, a;
		};
		Vector3<T> xyz;
		T xyzw[4];
	};

	OP_ALWAYS_INLINE constexpr Vector4() : x(0), y(0), z(0) {}
	OP_ALWAYS_INLINE constexpr Vector4(T xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
	OP_ALWAYS_INLINE constexpr Vector4(T x, T y, T z, T w) : r(x), g(y), b(z), a(w) {}
	OP_ALWAYS_INLINE constexpr Vector4(Vector3<T> xyz, T w = 0);

	static const Vector4<T> zero;
	static const Vector4<T> one;
	static const Vector4<T> infinite;

	OP_ALWAYS_INLINE Vector4 operator+(const Vector4& rhs) const;
	OP_ALWAYS_INLINE Vector4 operator-(const Vector4& rhs) const;
	OP_ALWAYS_INLINE Vector4 operator*(const Vector4& rhs) const;
	OP_ALWAYS_INLINE Vector4 operator/(const Vector4& rhs) const;
	OP_ALWAYS_INLINE void operator+=(const Vector4& rhs) { *this = *this + rhs; }
	OP_ALWAYS_INLINE void operator-=(const Vector4& rhs) { *this = *this - rhs; }
	OP_ALWAYS_INLINE void operator*=(const Vector4& rhs) { *this = *this * rhs; }
	OP_ALWAYS_INLINE void operator/=(const Vector4& rhs) { *this = *this / rhs; }
	OP_ALWAYS_INLINE Vector4 operator-() const { return { -x, -y, -z, -w }; }

	OP_ALWAYS_INLINE T dot(const Vector4& rhs) const;
	OP_ALWAYS_INLINE T len_sq() const { return dot(*this); }
	OP_ALWAYS_INLINE T len() const { return op::sqrt(len_sq()); }
	OP_ALWAYS_INLINE Option<Vector4<T>> normalized() const;

	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_zero() const { return *this == zero; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_near_zero() const;
	OP_NO_DISCARD OP_ALWAYS_INLINE bool has_nan() const;
	OP_NO_DISCARD OP_ALWAYS_INLINE bool has_infinite() const;

	template <typename D>
	OP_NO_DISCARD OP_ALWAYS_INLINE Vector4<D> cast() const {
		return { static_cast<D>(x), static_cast<D>(y), static_cast<D>(z), static_cast<D>(w) };
	}
};

OP_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/vector4.inl"

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Vector4;
OP_NAMESPACE_END
