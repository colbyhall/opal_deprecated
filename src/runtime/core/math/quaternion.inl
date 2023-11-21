// Copyright Colby Hall. All Rights Reserved.

OP_CORE_NAMESPACE_BEGIN

template <typename T>
const Quaternion<T> Quaternion<T>::identity = {};

template <typename T>
Quaternion<T> Quaternion<T>::from_axis_angle(const Vector3<T>& axis, T theta) {
	const auto half_theta = theta / (T)2;
	const auto s = core::sin(half_theta);
	const auto c = core::cos(half_theta);
	return Quaternion{ s * axis.x, s * axis.y, s * axis.z, c };
}

template <typename T>
Quaternion<T> Quaternion<T>::from_euler(T pitch, T yaw, T roll) {
	const auto rads_div_by_2 = (T)core::deg_to_rad / (T)2;

	const auto pitch1 = core::fmod(pitch, (T)360);
	const auto yaw1 = core::fmod(yaw, (T)360);
	const auto roll1 = core::fmod(roll, (T)360);

	const auto sp = core::sin(pitch1 * rads_div_by_2);
	const auto cp = core::cos(pitch1 * rads_div_by_2);

	const auto sy = core::sin(yaw1 * rads_div_by_2);
	const auto cy = core::cos(yaw1 * rads_div_by_2);

	const auto sr = core::sin(roll1 * rads_div_by_2);
	const auto cr = core::cos(roll1 * rads_div_by_2);

	return Quaternion{ cr * sp * sy - sr * cp * cy,
					   -cr * sp * cy - sr * cp * sy,
					   cr * cp * sy - sr * sp * cy,
					   cr * cp * cy + sr * sp * sy };
}

template <typename T>
Option<Quaternion<T>> Quaternion<T>::normalized() const {
	if (is_near_zero()) return Option<Quaternion>{};
	const auto inverse = (T)1 / len();
	return Quaternion{ i * inverse, j * inverse, k * inverse, w * inverse };
}

template <typename T>
Vector3<T> Quaternion<T>::rotate(const Vector3<T>& xyz) const {
	const Vector3<T> this_xyz = { i, j, k };
	const auto t = this_xyz.cross(xyz) * (T)2;
	return xyz + (t * w) + this_xyz.cross(t);
}

template <typename T>
Quaternion<T> Quaternion<T>::operator*(const Quaternion<T>& rhs) const {
	const auto result = Quaternion<T>{ (((w * rhs.i) + (i * rhs.w)) + (j * rhs.k)) - (k * rhs.j),
									   (((w * rhs.j) + (j * rhs.w)) + (k * rhs.i)) - (i * rhs.k),
									   (((w * rhs.k) + (k * rhs.w)) + (i * rhs.j)) - (j * rhs.i),
									   (((w * rhs.w) - (i * rhs.i)) - (j * rhs.j)) - (k * rhs.k) };
	return result.normalize().unwrap();
}

OP_CORE_NAMESPACE_END
