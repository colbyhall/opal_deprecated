// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vec2.h"

EU_CORE_NAMESPACE_BEGIN

template <typename T>
struct alignas(16) Vec3 {
	union {
		struct {
			T x, y, z;
		};
		struct {
			T width, depth, height;
		};
		struct {
			T u, v, w;
		};
		Vec2<f32> xy;
		T xyz[3];
	};

	EU_ALWAYS_INLINE constexpr Vec3() : x(0), y(0), z(0) {}
	EU_ALWAYS_INLINE constexpr Vec3(T xyz) : x(xyz), y(xyz), z(xyz) {}
	EU_ALWAYS_INLINE constexpr Vec3(T x, T y, T z) : u(x), v(y), w(z) {}
	EU_ALWAYS_INLINE constexpr Vec3(Vec2<T> xy, T z = 0);

	static const Vec3<T> zero;
	static const Vec3<T> one;
	static const Vec3<T> infinite;

	static const Vec3<T> forward;
	static const Vec3<T> right;
	static const Vec3<T> up;

	EU_ALWAYS_INLINE Vec3 operator+(const Vec3& rhs) const;
	EU_ALWAYS_INLINE Vec3 operator-(const Vec3& rhs) const;
	EU_ALWAYS_INLINE Vec3 operator*(const Vec3& rhs) const;
	EU_ALWAYS_INLINE Vec3 operator/(const Vec3& rhs) const;
	EU_ALWAYS_INLINE void operator+=(const Vec3& rhs) { *this = *this + rhs; }
	EU_ALWAYS_INLINE void operator-=(const Vec3& rhs) { *this = *this - rhs; }
	EU_ALWAYS_INLINE void operator*=(const Vec3& rhs) { *this = *this * rhs; }
	EU_ALWAYS_INLINE void operator/=(const Vec3& rhs) { *this = *this / rhs; }
	EU_ALWAYS_INLINE Vec3 operator-() const { return {-x, -y, -z}; }

	EU_ALWAYS_INLINE T dot(const Vec3& rhs) const;
	EU_ALWAYS_INLINE Vec3<T> cross(const Vec3& rhs) const;
	EU_ALWAYS_INLINE T len_sq() const { return dot(*this); }
	EU_ALWAYS_INLINE T len() const { return eu::sqrt(len_sq()); }
	EU_ALWAYS_INLINE Option<Vec3<T>> normalized() const;

	EU_ALWAYS_INLINE bool is_zero() const { return *this == zero; }
	EU_ALWAYS_INLINE bool is_near_zero() const;
	EU_ALWAYS_INLINE bool has_nan() const;
	EU_ALWAYS_INLINE bool has_infinite() const;

	template <typename D>
	EU_ALWAYS_INLINE Vec3<D> cast() const {
		return {static_cast<D>(x), static_cast<D>(y), static_cast<D>(z)};
	}
};

EU_CORE_NAMESPACE_END

#include "core/math/vec3.inl"

EU_NAMESPACE_BEGIN

using Vec3f32 = core::Vec3<f32>;
using Vec3f64 = core::Vec3<f64>;
using Vec3i32 = core::Vec3<i32>;
using Vec3i64 = core::Vec3<i64>;
using Vec3u32 = core::Vec3<u32>;
using Vec3u64 = core::Vec3<u64>;

EU_NAMESPACE_END
