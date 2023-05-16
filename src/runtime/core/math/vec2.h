// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/math/math.h"

EU_CORE_NAMESPACE_BEGIN

template <typename T>
struct alignas(16) Vec2 {
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

	EU_ALWAYS_INLINE constexpr Vec2() : x(0), y(0) {}
	EU_ALWAYS_INLINE constexpr Vec2(T xy) : x(xy), y(xy) {}
	EU_ALWAYS_INLINE constexpr Vec2(T x, T y) : u(x), v(y) {}

	EU_ALWAYS_INLINE static Vec2<T> from_rad(T rad);

	static const Vec2<T> zero;
	static const Vec2<T> one;
	static const Vec2<T> infinite;

	static const Vec2<T> right;
	static const Vec2<T> up;

	EU_ALWAYS_INLINE Vec2 operator+(const Vec2& rhs) const;
	EU_ALWAYS_INLINE Vec2 operator-(const Vec2& rhs) const;
	EU_ALWAYS_INLINE Vec2 operator*(const Vec2& rhs) const;
	EU_ALWAYS_INLINE Vec2 operator/(const Vec2& rhs) const;
	EU_ALWAYS_INLINE void operator+=(const Vec2& rhs) { *this = *this + rhs; }
	EU_ALWAYS_INLINE void operator-=(const Vec2& rhs) { *this = *this - rhs; }
	EU_ALWAYS_INLINE void operator*=(const Vec2& rhs) { *this = *this * rhs; }
	EU_ALWAYS_INLINE void operator/=(const Vec2& rhs) { *this = *this / rhs; }
	EU_ALWAYS_INLINE Vec2 operator-() const { return {-x, -y}; }

	EU_ALWAYS_INLINE T dot(const Vec2<T>& rhs) const;
	EU_ALWAYS_INLINE T cross(const Vec2<T>& rhs) const;
	EU_ALWAYS_INLINE Vec2<T> perp() const;
	EU_ALWAYS_INLINE T len_sq() const { return dot(*this); }
	EU_ALWAYS_INLINE T len() const { return eu::sqrt(len_sq()); }
	EU_ALWAYS_INLINE Option<Vec2<T>> normalized() const;

	EU_ALWAYS_INLINE bool is_zero() const { return *this == zero; }
	EU_ALWAYS_INLINE bool is_near_zero() const;
	EU_ALWAYS_INLINE bool has_nan() const;
	EU_ALWAYS_INLINE bool has_infinite() const;

	template <typename D>
	EU_ALWAYS_INLINE Vec2<D> cast() const {
		return {static_cast<D>(x), static_cast<D>(y)};
	}
};

EU_CORE_NAMESPACE_END

#include "core/math/vec2.inl"

EU_NAMESPACE_BEGIN

using Vec2f32 = core::Vec2<f32>;
using Vec2f64 = core::Vec2<f64>;
using Vec2i32 = core::Vec2<i32>;
using Vec2i64 = core::Vec2<i64>;
using Vec2u32 = core::Vec2<u32>;
using Vec2u64 = core::Vec2<u64>;

EU_NAMESPACE_END
