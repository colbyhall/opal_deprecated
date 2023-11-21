// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/math/math.h"

OP_CORE_NAMESPACE_BEGIN

/**
 * A Vector that contains two elements.
 *
 * @tparam T A numeric type.
 */
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

	OP_ALWAYS_INLINE constexpr Vector2() : x(0), y(0) {}
	OP_ALWAYS_INLINE constexpr Vector2(T xy) : x(xy), y(xy) {}
	OP_ALWAYS_INLINE constexpr Vector2(T x, T y) : u(x), v(y) {}

	OP_ALWAYS_INLINE static Vector2<T> from_rad(T rad);

	static const Vector2<T> zero;
	static const Vector2<T> one;
	static const Vector2<T> infinite;

	static const Vector2<T> right;
	static const Vector2<T> up;

	OP_ALWAYS_INLINE Vector2 operator+(const Vector2& rhs) const;
	OP_ALWAYS_INLINE Vector2 operator-(const Vector2& rhs) const;
	OP_ALWAYS_INLINE Vector2 operator*(const Vector2& rhs) const;
	OP_ALWAYS_INLINE Vector2 operator/(const Vector2& rhs) const;
	OP_ALWAYS_INLINE void operator+=(const Vector2& rhs) { *this = *this + rhs; }
	OP_ALWAYS_INLINE void operator-=(const Vector2& rhs) { *this = *this - rhs; }
	OP_ALWAYS_INLINE void operator*=(const Vector2& rhs) { *this = *this * rhs; }
	OP_ALWAYS_INLINE void operator/=(const Vector2& rhs) { *this = *this / rhs; }
	OP_ALWAYS_INLINE Vector2 operator-() const { return { -x, -y }; }

	/**
	 * @return the cos of the angle between the two vectors.
	 */
	OP_ALWAYS_INLINE T dot(const Vector2<T>& rhs) const;

	/**
	 * @return the sin of the angle between the two vectors.
	 */
	OP_ALWAYS_INLINE T cross(const Vector2<T>& rhs) const;

	/**
	 * @returns a new vector that is perpendicular to this vector.
	 */
	OP_ALWAYS_INLINE Vector2<T> perp() const;

	/**
	 * @returns the length of the vector squared.
	 */
	OP_ALWAYS_INLINE T len_sq() const { return dot(*this); }

	/**
	 * @returns the length of the vector.
	 */
	OP_ALWAYS_INLINE T len() const { return op::sqrt(len_sq()); }

	/**
	 * @returns an option containing the normalized vector if the length is not zero.
	 */
	OP_ALWAYS_INLINE Option<Vector2<T>> normalized() const;

	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_zero() const { return *this == zero; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_near_zero() const;
	OP_NO_DISCARD OP_ALWAYS_INLINE bool has_nan() const;
	OP_NO_DISCARD OP_ALWAYS_INLINE bool has_infinite() const;

	/**
	 * Returns the minimum elements of two vectors.
	 *
	 * @param rhs The other vector to compare against.
	 * @return The minimum elements of the two vectors.
	 */
	OP_NO_DISCARD Vector2<T> min(const Vector2<T>& rhs) const { return { op::min(x, rhs.x), op::min(y, rhs.y) }; }

	/**
	 * Returns the maximum elements of two vectors.
	 *
	 * @param rhs The other vector to compare against.
	 * @return The maximum elements of the two vectors.
	 */
	OP_NO_DISCARD Vector2<T> max(const Vector2<T>& rhs) const { return { op::max(x, rhs.x), op::max(y, rhs.y) }; }

	/**
	 * Cast the vector to another type.
	 *
	 * @tparam D The type to cast to.
	 * @return A new vector with the casted type.
	 */
	template <typename D>
	OP_NO_DISCARD OP_ALWAYS_INLINE Vector2<D> cast() const {
		return { static_cast<D>(x), static_cast<D>(y) };
	}
};

OP_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/vector2.inl"

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Vector2;
OP_NAMESPACE_END
