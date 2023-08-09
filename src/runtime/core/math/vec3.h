// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vec2.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
struct alignas(16) Vec3 {
	union {
		struct {
			T x, y, z;
		};
		struct {
			T u, v, w;
		};
		Vec2<T> xy;
		T xyz[3];
	};

	GJ_ALWAYS_INLINE constexpr Vec3() : x(0), y(0), z(0) {}
	GJ_ALWAYS_INLINE constexpr Vec3(T xyz) : x(xyz), y(xyz), z(xyz) {}
	GJ_ALWAYS_INLINE constexpr Vec3(T x, T y, T z) : u(x), v(y), w(z) {}
	GJ_ALWAYS_INLINE constexpr Vec3(Vec2<T> xy, T z = 0);

	static const Vec3<T> zero;
	static const Vec3<T> one;
	static const Vec3<T> infinite;

	static const Vec3<T> forward;
	static const Vec3<T> right;
	static const Vec3<T> up;

	GJ_ALWAYS_INLINE Vec3 operator+(const Vec3& rhs) const;
	GJ_ALWAYS_INLINE Vec3 operator-(const Vec3& rhs) const;
	GJ_ALWAYS_INLINE Vec3 operator*(const Vec3& rhs) const;
	GJ_ALWAYS_INLINE Vec3 operator/(const Vec3& rhs) const;
	GJ_ALWAYS_INLINE void operator+=(const Vec3& rhs) { *this = *this + rhs; }
	GJ_ALWAYS_INLINE void operator-=(const Vec3& rhs) { *this = *this - rhs; }
	GJ_ALWAYS_INLINE void operator*=(const Vec3& rhs) { *this = *this * rhs; }
	GJ_ALWAYS_INLINE void operator/=(const Vec3& rhs) { *this = *this / rhs; }
	GJ_ALWAYS_INLINE Vec3 operator-() const { return { -x, -y, -z }; }

	GJ_ALWAYS_INLINE T dot(const Vec3& rhs) const;
	GJ_ALWAYS_INLINE Vec3<T> cross(const Vec3& rhs) const;
	GJ_ALWAYS_INLINE T len_sq() const { return dot(*this); }
	GJ_ALWAYS_INLINE T len() const { return gj::sqrt(len_sq()); }
	GJ_ALWAYS_INLINE Option<Vec3<T>> normalized() const;

	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_zero() const {
		return *this == zero;
	}
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_near_zero() const;
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool has_nan() const;
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool has_infinite() const;

	template <typename D>
	GJ_NO_DISCARD GJ_ALWAYS_INLINE Vec3<D> cast() const {
		return { static_cast<D>(x), static_cast<D>(y), static_cast<D>(z) };
	}
};

GJ_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/vec3.inl"

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using Vec3f32 = core::Vec3<f32>;
using Vec3f64 = core::Vec3<f64>;
using Vec3i32 = core::Vec3<i32>;
using Vec3i64 = core::Vec3<i64>;
using Vec3u32 = core::Vec3<u32>;
using Vec3u64 = core::Vec3<u64>;
GJ_NAMESPACE_END
