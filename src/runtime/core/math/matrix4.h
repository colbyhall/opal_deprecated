// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/quaternion.h"
#include "core/math/vector4.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
struct alignas(16) Matrix4 {
	union {
		struct {
			Vector4<T> x, y, z, w;
		};
		Vector4<T> columns[4];
		T elements[4 * 4];
	};

	constexpr Matrix4() : x(0), y(0), z(0), w(0) {}

	static constexpr Matrix4 from_columns(
		const Vector4<T>& x,
		const Vector4<T>& y,
		const Vector4<T>& z,
		const Vector4<T>& w
	);
	static constexpr Matrix4 from_rows(
		const Vector4<T>& x,
		const Vector4<T>& y,
		const Vector4<T>& z,
		const Vector4<T>& w
	);
	static constexpr Matrix4 orthographic(T width, T height, T near, T far);
	static Matrix4 perspective(T fov, T aspect_ratio, T near, T far);
	static Matrix4 transform(
		const Vector3<T>& position,
		const Quaternion<T>& rotation,
		const Vector3<T>& scale
	);

	static const Matrix4 identity;

	Option<Matrix4> inverse() const;
	Vector4<T> row(usize index) const;

	Matrix4 operator*(const Matrix4& rhs) const;
	Vector4<T> operator*(const Vector4<T>& rhs) const;

private:
	inline constexpr Matrix4(
		const Vector4<T>& _x,
		const Vector4<T>& _y,
		const Vector4<T>& _z,
		const Vector4<T>& _w
	)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w) {}
};

GJ_CORE_NAMESPACE_END

// Include the implementation
#include "core/math/matrix4.inl"

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::Matrix4;
GJ_NAMESPACE_END
