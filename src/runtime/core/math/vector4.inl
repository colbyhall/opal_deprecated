// Copyright Colby Hall. All Rights Reserved.

OP_CORE_NAMESPACE_BEGIN

template <typename T>
const Vector4<T> Vector4<T>::zero = { 0 };

template <typename T>
const Vector4<T> Vector4<T>::one = { 1 };

template <typename T>
const Vector4<T> Vector4<T>::infinite = { op::infinity<T> };

template <typename T>
OP_ALWAYS_INLINE constexpr Vector4<T>::Vector4(Vector3<T> xyz, T w) : r(xyz.x)
																	, g(xyz.y)
																	, b(xyz.z)
																	, a(w) {}

template <typename T>
OP_ALWAYS_INLINE Vector4<T> Vector4<T>::operator+(const Vector4<T>& rhs) const {
	return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
}

template <typename T>
OP_ALWAYS_INLINE Vector4<T> Vector4<T>::operator-(const Vector4<T>& rhs) const {
	return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
}

template <typename T>
OP_ALWAYS_INLINE Vector4<T> Vector4<T>::operator*(const Vector4<T>& rhs) const {
	return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
}

template <typename T>
OP_ALWAYS_INLINE Vector4<T> Vector4<T>::operator/(const Vector4<T>& rhs) const {
	return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w };
}

template <typename T>
OP_ALWAYS_INLINE T Vector4<T>::dot(const Vector4<T>& rhs) const {
	return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

template <typename T>
OP_ALWAYS_INLINE Option<Vector4<T>> Vector4<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return nullptr;
}

template <typename T>
OP_ALWAYS_INLINE bool Vector4<T>::is_near_zero() const {
	return op::core::is_near_zero(len_sq());
}

template <typename T>
OP_ALWAYS_INLINE bool Vector4<T>::has_nan() const {
	return x == op::nan || y == op::nan || z == op::nan || w == op::nan;
}

template <typename T>
OP_ALWAYS_INLINE bool Vector4<T>::has_infinite() const {
	return x == op::infinity || y == op::infinity || z == op::infinity || w == op::infinity;
}

OP_CORE_NAMESPACE_END
