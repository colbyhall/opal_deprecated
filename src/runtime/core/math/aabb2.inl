// Copyright Colby Hall. All Rights Reserved.

#include "core/math/aabb2.h"

SF_CORE_NAMESPACE_BEGIN

template <typename T>
AABB2<T> AABB2<T>::from_center(const Vector2<T>& center, const Vector2<T>& half_size) {
	SF_ASSERT(half_size.x >= 0 && half_size.y >= 0);

	auto result = AABB2<T>();
	result.m_center = center;
	result.m_half_size = half_size;
	return result;
}

template <typename T>
AABB2<T> AABB2<T>::from_min_max(const Vector2<T>& min, const Vector2<T>& max) {
	const auto center = min + max * (T)0.5;
	const auto half_size = (max - min) * (T)0.5;

	auto result = AABB2<T>();
	result.m_center = center;
	result.m_half_size = half_size;
	return result;
}

template <typename T>
Option<AABB2<T>> AABB2<T>::overlaps(const AABB2<T>& other) const {
	// Calculate the AABB resulting from the intersection of the two AABBs
	const auto result_min = min().max(other.min());
	const auto result_max = max().min(other.max());

	// Check for non-overlapping AABBs
	if (result_min.x > result_max.x || result_min.y > result_max.y) {
		return nullopt;
	}

	return AABB2<T>::from_min_max(result_min, result_max);
}

template <typename T>
bool AABB2<T>::encompasses(const Vector2<T>& point) const {
	return min().max(point) == point && max().min(point) == point;
}

SF_CORE_NAMESPACE_END