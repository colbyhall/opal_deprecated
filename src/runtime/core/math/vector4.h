// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector3.h"

GJ_CORE_NAMESPACE_BEGIN

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

	GJ_ALWAYS_INLINE constexpr Vector4() : x(0), y(0), z(0) {}
	GJ_ALWAYS_INLINE constexpr Vector4(T xyzw)
		: x(xyzw)
		, y(xyzw)
		, z(xyzw)
		, w(xyzw) {}
	GJ_ALWAYS_INLINE constexpr Vector4(T x, T y, T z, T w)
		: r(x)
		, g(y)
		, b(z)
		, a(w) {}
	GJ_ALWAYS_INLINE constexpr Vector4(Vector3<T> xyz, T w = 0);

	static const Vector4<T> zero;
	static const Vector4<T> one;
	static const Vector4<T> infinite;

	GJ_ALWAYS_INLINE Vector4 operator+(const Vector4& rhs) const;
	GJ_ALWAYS_INLINE Vector4 operator-(const Vector4& rhs) const;
	GJ_ALWAYS_INLINE Vector4 operator*(const Vector4& rhs) const;
	GJ_ALWAYS_INLINE Vector4 operator/(const Vector4& rhs) const;
	GJ_ALWAYS_INLINE void operator+=(const Vector4& rhs) {
		*this = *this + rhs;
	}
	GJ_ALWAYS_INLINE void operator-=(const Vector4& rhs) {
		*this = *this - rhs;
	}
	GJ_ALWAYS_INLINE void operator*=(const Vector4& rhs) {
		*this = *this * rhs;
	}
	GJ_ALWAYS_INLINE void operator/=(const Vector4& rhs) {
		*this = *this / rhs;
	}
	GJ_ALWAYS_INLINE Vector4 operator-() const { return { -x, -y, -z, -w }; }

	GJ_ALWAYS_INLINE T dot(const Vector4& rhs) const;
	GJ_ALWAYS_INLINE T len_sq() const { return dot(*this); }
	GJ_ALWAYS_INLINE T len() const { return gj::sqrt(len_sq()); }
	GJ_ALWAYS_INLINE Option<Vector4<T>> normalized() const;

	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_zero() const {
		return *this == zero;
	}
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_near_zero() const;
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool has_nan() const;
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool has_infinite() const;

	template <typename D>
	GJ_NO_DISCARD GJ_ALWAYS_INLINE Vector4<D> cast() const {
		return { static_cast<D>(x),
				 static_cast<D>(y),
				 static_cast<D>(z),
				 static_cast<D>(w) };
	}
};

GJ_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/vector4.inl"

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::Vector4;
GJ_NAMESPACE_END
