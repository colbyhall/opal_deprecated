// Copyright Colby Hall. All Rights Reserved.

SF_CORE_NAMESPACE_BEGIN

template <typename T>
const Vector3<T> Vector3<T>::zero = { 0 };

template <typename T>
const Vector3<T> Vector3<T>::one = { 1 };

template <typename T>
const Vector3<T> Vector3<T>::infinite = { sf::infinity<T> };

template <typename T>
const Vector3<T> Vector3<T>::forward = { 1, 0, 0 };

template <typename T>
const Vector3<T> Vector3<T>::right = { 0, 1, 0 };

template <typename T>
const Vector3<T> Vector3<T>::up = { 0, 0, 1 };

template <typename T>
SF_ALWAYS_INLINE constexpr Vector3<T>::Vector3(Vector2<T> xy, T z)
	: u(xy.x)
	, v(xy.y)
	, w(z) {}

template <typename T>
SF_ALWAYS_INLINE Vector3<T> Vector3<T>::operator+(const Vector3<T>& rhs) const {
	return { x + rhs.x, y + rhs.y, z + rhs.z };
}

template <typename T>
SF_ALWAYS_INLINE Vector3<T> Vector3<T>::operator-(const Vector3<T>& rhs) const {
	return { x - rhs.x, y - rhs.y, z - rhs.z };
}

template <typename T>
SF_ALWAYS_INLINE Vector3<T> Vector3<T>::operator*(const Vector3<T>& rhs) const {
	return { x * rhs.x, y * rhs.y, z * rhs.z };
}

template <typename T>
SF_ALWAYS_INLINE Vector3<T> Vector3<T>::operator/(const Vector3<T>& rhs) const {
	return { x / rhs.x, y / rhs.y, z / rhs.z };
}

template <typename T>
SF_ALWAYS_INLINE T Vector3<T>::dot(const Vector3<T>& rhs) const {
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

template <typename T>
SF_ALWAYS_INLINE Vector3<T> Vector3<T>::cross(const Vector3<T>& rhs) const {
	return { y * rhs.z - rhs.y * z,
			 z * rhs.x - rhs.z * x,
			 x * rhs.y - rhs.x * y };
}

template <typename T>
SF_ALWAYS_INLINE Option<Vector3<T>> Vector3<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return nullptr;
}

template <typename T>
SF_ALWAYS_INLINE bool Vector3<T>::is_near_zero() const {
	return sf::core::is_near_zero(len_sq());
}

template <typename T>
SF_ALWAYS_INLINE bool Vector3<T>::has_nan() const {
	return x == sf::nan || y == sf::nan || z == sf::nan;
}

template <typename T>
SF_ALWAYS_INLINE bool Vector3<T>::has_infinite() const {
	return x == sf::infinity || y == sf::infinity || z == sf::infinity;
}

SF_CORE_NAMESPACE_END
