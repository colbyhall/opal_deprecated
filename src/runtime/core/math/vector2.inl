// Copyright Colby Hall. All Rights Reserved.

OP_CORE_NAMESPACE_BEGIN

template <Number T>
OP_ALWAYS_INLINE Vector2<T> Vector2<T>::from_rad(T theta) {
	return { op::sin(theta), op::cos(theta) };
}

template <Number T>
const Vector2<T> Vector2<T>::zero = { 0 };

template <Number T>
const Vector2<T> Vector2<T>::one = { 1 };

template <Number T>
const Vector2<T> Vector2<T>::infinite = { op::infinity<T> };

template <Number T>
const Vector2<T> Vector2<T>::right = { 1, 0 };

template <Number T>
const Vector2<T> Vector2<T>::up = { 0, 1 };

template <Number T>
OP_ALWAYS_INLINE Vector2<T> Vector2<T>::operator+(const Vector2<T>& rhs) const {
	return { x + rhs.x, y + rhs.y };
}

template <Number T>
OP_ALWAYS_INLINE Vector2<T> Vector2<T>::operator-(const Vector2<T>& rhs) const {
	return { x - rhs.x, y - rhs.y };
}

template <Number T>
OP_ALWAYS_INLINE Vector2<T> Vector2<T>::operator*(const Vector2<T>& rhs) const {
	return { x * rhs.x, y * rhs.y };
}

template <Number T>
OP_ALWAYS_INLINE Vector2<T> Vector2<T>::operator/(const Vector2<T>& rhs) const {
	return { x / rhs.x, y / rhs.y };
}

template <Number T>
OP_ALWAYS_INLINE T Vector2<T>::dot(const Vector2<T>& rhs) const {
	return x * rhs.x + y * rhs.y;
}

template <Number T>
OP_ALWAYS_INLINE T Vector2<T>::cross(const Vector2<T>& rhs) const {
	return x * rhs.y - y * rhs.x;
}

template <Number T>
OP_ALWAYS_INLINE Vector2<T> Vector2<T>::perp() const {
	return { y, -x };
}

template <Number T>
OP_ALWAYS_INLINE Option<Vector2<T>> Vector2<T>::normalized() const {
	if (!is_near_zero()) {
		return *this / len();
	}
	return op::nullopt;
}

template <Number T>
OP_ALWAYS_INLINE bool Vector2<T>::is_near_zero() const {
	return op::core::is_near_zero(len_sq());
}

template <Number T>
OP_ALWAYS_INLINE bool Vector2<T>::has_nan() const {
	return x == op::nan || y == op::nan;
}

template <Number T>
OP_ALWAYS_INLINE bool Vector2<T>::has_infinite() const {
	return x == op::infinity || y == op::infinity;
}

OP_CORE_NAMESPACE_END
