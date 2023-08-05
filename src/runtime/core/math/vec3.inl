// Copyright Colby Hall. All Rights Reserved.

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
const Vec3<T> Vec3<T>::zero = { 0 };

template <typename T>
const Vec3<T> Vec3<T>::one = { 1 };

template <typename T>
const Vec3<T> Vec3<T>::infinite = { gj::infinity<T> };

template <typename T>
const Vec3<T> Vec3<T>::forward = { 1, 0, 0 };

template <typename T>
const Vec3<T> Vec3<T>::right = { 0, 1, 0 };

template <typename T>
const Vec3<T> Vec3<T>::up = { 0, 0, 1 };

template <typename T>
GJ_ALWAYS_INLINE constexpr Vec3<T>::Vec3(Vec2<T> xy, T z) : u(xy.x)
														  , v(xy.y)
														  , w(z) {}

template <typename T>
GJ_ALWAYS_INLINE Vec3<T> Vec3<T>::operator+(const Vec3<T>& rhs) const {
	return { x + rhs.x, y + rhs.y, z + rhs.z };
}

template <typename T>
GJ_ALWAYS_INLINE Vec3<T> Vec3<T>::operator-(const Vec3<T>& rhs) const {
	return { x - rhs.x, y - rhs.y, z - rhs.z };
}

template <typename T>
GJ_ALWAYS_INLINE Vec3<T> Vec3<T>::operator*(const Vec3<T>& rhs) const {
	return { x * rhs.x, y * rhs.y, z * rhs.z };
}

template <typename T>
GJ_ALWAYS_INLINE Vec3<T> Vec3<T>::operator/(const Vec3<T>& rhs) const {
	return { x / rhs.x, y / rhs.y, z / rhs.z };
}

template <typename T>
GJ_ALWAYS_INLINE T Vec3<T>::dot(const Vec3<T>& rhs) const {
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

template <typename T>
GJ_ALWAYS_INLINE Vec3<T> Vec3<T>::cross(const Vec3<T>& rhs) const {
	return { y * rhs.z - rhs.y * z, z * rhs.x - rhs.z * x, x * rhs.y - rhs.x * y };
}

template <typename T>
GJ_ALWAYS_INLINE Option<Vec3<T>> Vec3<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return nullptr;
}

template <typename T>
GJ_ALWAYS_INLINE bool Vec3<T>::is_near_zero() const {
	return gj::core::is_near_zero(len_sq());
}

template <typename T>
GJ_ALWAYS_INLINE bool Vec3<T>::has_nan() const {
	return x == gj::nan || y == gj::nan || z == gj::nan;
}

template <typename T>
GJ_ALWAYS_INLINE bool Vec3<T>::has_infinite() const {
	return x == gj::infinity || y == gj::infinity || z == gj::infinity;
}

GJ_CORE_NAMESPACE_END
