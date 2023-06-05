// Copyright Colby Hall. All Rights Reserved.

EU_CORE_NAMESPACE_BEGIN

template <typename T>
EU_ALWAYS_INLINE Vec2<T> Vec2<T>::from_rad(T theta) {
	return { eu::sin(theta), eu::cos(theta) };
}

template <typename T>
const Vec2<T> Vec2<T>::zero = { 0 };

template <typename T>
const Vec2<T> Vec2<T>::one = { 1 };

template <typename T>
const Vec2<T> Vec2<T>::infinite = { eu::infinity };

template <typename T>
const Vec2<T> Vec2<T>::right = { 1, 0 };

template <typename T>
const Vec2<T> Vec2<T>::up = { 0, 1 };

template <typename T>
EU_ALWAYS_INLINE Vec2<T> Vec2<T>::operator+(const Vec2<T>& rhs) const {
	return { x + rhs.x, y + rhs.y };
}

template <typename T>
EU_ALWAYS_INLINE Vec2<T> Vec2<T>::operator-(const Vec2<T>& rhs) const {
	return { x - rhs.x, y - rhs.y };
}

template <typename T>
EU_ALWAYS_INLINE Vec2<T> Vec2<T>::operator*(const Vec2<T>& rhs) const {
	return { x * rhs.x, y * rhs.y };
}

template <typename T>
EU_ALWAYS_INLINE Vec2<T> Vec2<T>::operator/(const Vec2<T>& rhs) const {
	return { x / rhs.x, y / rhs.y };
}

template <typename T>
EU_ALWAYS_INLINE T Vec2<T>::dot(const Vec2<T>& rhs) const {
	return x * rhs.x + y * rhs.y;
}

template <typename T>
EU_ALWAYS_INLINE T Vec2<T>::cross(const Vec2<T>& rhs) const {
	return x * rhs.y - y * rhs.x;
}

template <typename T>
EU_ALWAYS_INLINE Vec2<T> Vec2<T>::perp() const {
	return { y, -x };
}

template <typename T>
EU_ALWAYS_INLINE Option<Vec2<T>> Vec2<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return eu::none;
}

template <typename T>
EU_ALWAYS_INLINE bool Vec2<T>::is_near_zero() const {
	return eu::core::is_near_zero(len_sq());
}

template <typename T>
EU_ALWAYS_INLINE bool Vec2<T>::has_nan() const {
	return x == eu::nan || y == eu::nan;
}

template <typename T>
EU_ALWAYS_INLINE bool Vec2<T>::has_infinite() const {
	return x == eu::infinity || y == eu::infinity;
}

EU_CORE_NAMESPACE_END
