// Copyright Colby Hall. All Rights Reserved.

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
GJ_ALWAYS_INLINE Vec2<T> Vec2<T>::from_rad(T theta) {
	return { gj::sin(theta), gj::cos(theta) };
}

template <typename T>
const Vec2<T> Vec2<T>::zero = { 0 };

template <typename T>
const Vec2<T> Vec2<T>::one = { 1 };

template <typename T>
const Vec2<T> Vec2<T>::infinite = { gj::infinity };

template <typename T>
const Vec2<T> Vec2<T>::right = { 1, 0 };

template <typename T>
const Vec2<T> Vec2<T>::up = { 0, 1 };

template <typename T>
GJ_ALWAYS_INLINE Vec2<T> Vec2<T>::operator+(const Vec2<T>& rhs) const {
	return { x + rhs.x, y + rhs.y };
}

template <typename T>
GJ_ALWAYS_INLINE Vec2<T> Vec2<T>::operator-(const Vec2<T>& rhs) const {
	return { x - rhs.x, y - rhs.y };
}

template <typename T>
GJ_ALWAYS_INLINE Vec2<T> Vec2<T>::operator*(const Vec2<T>& rhs) const {
	return { x * rhs.x, y * rhs.y };
}

template <typename T>
GJ_ALWAYS_INLINE Vec2<T> Vec2<T>::operator/(const Vec2<T>& rhs) const {
	return { x / rhs.x, y / rhs.y };
}

template <typename T>
GJ_ALWAYS_INLINE T Vec2<T>::dot(const Vec2<T>& rhs) const {
	return x * rhs.x + y * rhs.y;
}

template <typename T>
GJ_ALWAYS_INLINE T Vec2<T>::cross(const Vec2<T>& rhs) const {
	return x * rhs.y - y * rhs.x;
}

template <typename T>
GJ_ALWAYS_INLINE Vec2<T> Vec2<T>::perp() const {
	return { y, -x };
}

template <typename T>
GJ_ALWAYS_INLINE Option<Vec2<T>> Vec2<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return gj::none;
}

template <typename T>
GJ_ALWAYS_INLINE bool Vec2<T>::is_near_zero() const {
	return gj::core::is_near_zero(len_sq());
}

template <typename T>
GJ_ALWAYS_INLINE bool Vec2<T>::has_nan() const {
	return x == gj::nan || y == gj::nan;
}

template <typename T>
GJ_ALWAYS_INLINE bool Vec2<T>::has_infinite() const {
	return x == gj::infinity || y == gj::infinity;
}

GJ_CORE_NAMESPACE_END
