// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector3.h"

SF_CORE_NAMESPACE_BEGIN

template <typename T>
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

	SF_ALWAYS_INLINE constexpr Vector4() : x(0), y(0), z(0) {}
	SF_ALWAYS_INLINE constexpr Vector4(T xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
	SF_ALWAYS_INLINE constexpr Vector4(T x, T y, T z, T w) : r(x), g(y), b(z), a(w) {}
	SF_ALWAYS_INLINE constexpr Vector4(Vector3<T> xyz, T w = 0);

	static const Vector4<T> zero;
	static const Vector4<T> one;
	static const Vector4<T> infinite;

	SF_ALWAYS_INLINE Vector4 operator+(const Vector4& rhs) const;
	SF_ALWAYS_INLINE Vector4 operator-(const Vector4& rhs) const;
	SF_ALWAYS_INLINE Vector4 operator*(const Vector4& rhs) const;
	SF_ALWAYS_INLINE Vector4 operator/(const Vector4& rhs) const;
	SF_ALWAYS_INLINE void operator+=(const Vector4& rhs) { *this = *this + rhs; }
	SF_ALWAYS_INLINE void operator-=(const Vector4& rhs) { *this = *this - rhs; }
	SF_ALWAYS_INLINE void operator*=(const Vector4& rhs) { *this = *this * rhs; }
	SF_ALWAYS_INLINE void operator/=(const Vector4& rhs) { *this = *this / rhs; }
	SF_ALWAYS_INLINE Vector4 operator-() const { return { -x, -y, -z, -w }; }

	SF_ALWAYS_INLINE T dot(const Vector4& rhs) const;
	SF_ALWAYS_INLINE T len_sq() const { return dot(*this); }
	SF_ALWAYS_INLINE T len() const { return sf::sqrt(len_sq()); }
	SF_ALWAYS_INLINE Option<Vector4<T>> normalized() const;

	SF_NO_DISCARD SF_ALWAYS_INLINE bool is_zero() const { return *this == zero; }
	SF_NO_DISCARD SF_ALWAYS_INLINE bool is_near_zero() const;
	SF_NO_DISCARD SF_ALWAYS_INLINE bool has_nan() const;
	SF_NO_DISCARD SF_ALWAYS_INLINE bool has_infinite() const;

	template <typename D>
	SF_NO_DISCARD SF_ALWAYS_INLINE Vector4<D> cast() const {
		return { static_cast<D>(x), static_cast<D>(y), static_cast<D>(z), static_cast<D>(w) };
	}
};

SF_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/vector4.inl"

// Export to sf namespace
SF_NAMESPACE_BEGIN
using core::Vector4;
SF_NAMESPACE_END
