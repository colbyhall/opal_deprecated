// Copyright Colby Hall. All Rights Reserved.

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
const Vector4<T> Vector4<T>::zero = { 0 };

template <typename T>
const Vector4<T> Vector4<T>::one = { 1 };

template <typename T>
const Vector4<T> Vector4<T>::infinite = { gj::infinity<T> };

template <typename T>
GJ_ALWAYS_INLINE constexpr Vector4<T>::Vector4(Vector3<T> xyz, T w)
	: r(xyz.x)
	, g(xyz.y)
	, b(xyz.z)
	, a(w) {}

template <typename T>
GJ_ALWAYS_INLINE Vector4<T> Vector4<T>::operator+(const Vector4<T>& rhs) const {
	return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
}

template <typename T>
GJ_ALWAYS_INLINE Vector4<T> Vector4<T>::operator-(const Vector4<T>& rhs) const {
	return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
}

template <typename T>
GJ_ALWAYS_INLINE Vector4<T> Vector4<T>::operator*(const Vector4<T>& rhs) const {
	return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
}

template <typename T>
GJ_ALWAYS_INLINE Vector4<T> Vector4<T>::operator/(const Vector4<T>& rhs) const {
	return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w };
}

template <typename T>
GJ_ALWAYS_INLINE T Vector4<T>::dot(const Vector4<T>& rhs) const {
	return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

template <typename T>
GJ_ALWAYS_INLINE Option<Vector4<T>> Vector4<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return nullptr;
}

template <typename T>
GJ_ALWAYS_INLINE bool Vector4<T>::is_near_zero() const {
	return gj::core::is_near_zero(len_sq());
}

template <typename T>
GJ_ALWAYS_INLINE bool Vector4<T>::has_nan() const {
	return x == gj::nan || y == gj::nan || z == gj::nan || w == gj::nan;
}

template <typename T>
GJ_ALWAYS_INLINE bool Vector4<T>::has_infinite() const {
	return x == gj::infinity || y == gj::infinity || z == gj::infinity ||
		   w == gj::infinity;
}

GJ_CORE_NAMESPACE_END
