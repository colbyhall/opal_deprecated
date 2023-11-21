// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector2.h"

SF_CORE_NAMESPACE_BEGIN

/**
 * 2D Axis-aligned bounding box
 *
 * @tparam T A numeric type.
 */
template <typename T>
class AABB2 {
public:
	AABB2() = default;

	static AABB2<T> from_center(const Vector2<T>& center, const Vector2<T>& half_size);
	static AABB2<T> from_min_max(const Vector2<T>& min, const Vector2<T>& max);

	SF_NO_DISCARD SF_ALWAYS_INLINE Vector2<T> center() const { return m_center; }
	SF_NO_DISCARD SF_ALWAYS_INLINE Vector2<T> half_size() const { return m_half_size; }

	SF_NO_DISCARD SF_ALWAYS_INLINE Vector2<T> min() const { return m_center - m_half_size; }
	SF_NO_DISCARD SF_ALWAYS_INLINE Vector2<T> max() const { return m_center + m_half_size; }

	/** @returns an option containing the overlapping AABB if the two AABBs overlap, otherwise an empty option. */
	SF_NO_DISCARD Option<AABB2<T>> overlaps(const AABB2<T>& other) const;

	/** @return true if a point is within the AABB. */
	SF_NO_DISCARD bool encompasses(const Vector2<T>& point) const;

private:
	Vector2<T> m_center;
	Vector2<T> m_half_size;
};

SF_CORE_NAMESPACE_END

#include "core/math/aabb2.inl"

// Export to SF namespace
SF_NAMESPACE_BEGIN
using core::AABB2;
SF_NAMESPACE_END