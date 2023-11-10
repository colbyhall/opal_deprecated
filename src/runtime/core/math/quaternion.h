// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector3.h"

SF_CORE_NAMESPACE_BEGIN

template <typename T>
struct Quaternion {
	T i, j, k, w;

	SF_ALWAYS_INLINE constexpr Quaternion() : i(0), j(0), k(0), w(1) {}
	SF_ALWAYS_INLINE explicit Quaternion(T _i, T _j, T _k, T _w) : i(_i), j(_j), k(_k), w(_w) {}
	static Quaternion from_axis_angle(const Vector3<T>& axis, T theta);
	static Quaternion from_euler(T pitch, T yaw, T roll);

	static const Quaternion identity;

	Option<Quaternion> normalized() const;
	SF_ALWAYS_INLINE Quaternion inverse() const { return Quaternion{ -i, -j, -k, w }; }
	Vector3<T> rotate(const Vector3<T>& xyz) const;

	SF_ALWAYS_INLINE T len_sq() const { return i * i + j * j + k * k + w * w; }
	SF_ALWAYS_INLINE T len() const { return core::sqrt(len_sq()); }
	SF_ALWAYS_INLINE bool is_near_zero() const { return core::is_near_zero(len_sq()); }

	Quaternion operator*(const Quaternion& rhs) const;
};

SF_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/quaternion.inl"

// Export to sf namespace
SF_NAMESPACE_BEGIN
using core::Quaternion;
SF_NAMESPACE_END
