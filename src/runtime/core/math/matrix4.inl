// Copyright Colby Hall. All Rights Reserved.

OP_CORE_NAMESPACE_BEGIN

template <Number T>
const Matrix4<T> Matrix4<T>::identity =
	Matrix4<T>::from_columns({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 });

template <Number T>
constexpr Matrix4<T>
Matrix4<T>::from_columns(const Vector4<T>& x, const Vector4<T>& y, const Vector4<T>& z, const Vector4<T>& w) {
	return { x, y, z, w };
}

template <Number T>
constexpr Matrix4<T>
Matrix4<T>::from_rows(const Vector4<T>& x, const Vector4<T>& y, const Vector4<T>& z, const Vector4<T>& w) {
	const Vector4<T> x_col = { x.x, y.x, z.x, w.x };
	const Vector4<T> y_col = { x.y, y.y, z.y, w.y };
	const Vector4<T> z_col = { x.z, y.z, z.z, w.z };
	const Vector4<T> w_col = { x.w, y.w, z.w, w.w };
	return { x_col, y_col, z_col, w_col };
}

template <Number T>
constexpr Matrix4<T> Matrix4<T>::orthographic(T width, T height, T near, T far) {
	// 0 - 1 z clipping
	auto result = Matrix4::identity;
	result.x.x = (T)2 / width;
	result.y.y = (T)2 / height;
	result.z.z = (T)1 / (far - near);

	result.w.z = near / (far - near);

	return result;
}

template <Number T>
Matrix4<T> Matrix4<T>::perspective(T fov, T aspect_ratio, T near, T far) {
	const auto cotan = (T)1 / core::tan((fov * core::deg_to_rad) / (T)2);

	auto result = Matrix4::identity;
	result.x.x = cotan / aspect_ratio;
	result.y.y = cotan;
	result.z.w = -1;

	result.z.z = far / (near - far);
	result.w.z = -(far * near) / (far - near);

	result.w.w = 0;

	return result;
}

template <Number T>
Matrix4<T> Matrix4<T>::transform(const Vector3<T>& position, const Quaternion<T>& rotation, const Vector3<T>& scale) {
	Matrix4 result;
	result.x = { rotation.rotate(Vector3<T>::forward) * scale.x, 0 };
	result.y = { rotation.rotate(Vector3<T>::right) * scale.y, 0 };
	result.z = { rotation.rotate(Vector3<T>::up) * scale.z, 0 };
	result.w = { position, 1 };
	return result;
}

template <Number T>
Option<Matrix4<T>> Matrix4<T>::inverse() const {
	Matrix4 result = {};
	result.elements[0] = elements[5] * elements[10] * elements[15] - elements[5] * elements[11] * elements[14] -
						 elements[9] * elements[6] * elements[15] + elements[9] * elements[7] * elements[14] +
						 elements[13] * elements[6] * elements[11] - elements[13] * elements[7] * elements[10];

	result.elements[4] = -elements[4] * elements[10] * elements[15] + elements[4] * elements[11] * elements[14] +
						 elements[8] * elements[6] * elements[15] - elements[8] * elements[7] * elements[14] -
						 elements[12] * elements[6] * elements[11] + elements[12] * elements[7] * elements[10];

	result.elements[8] = elements[4] * elements[9] * elements[15] - elements[4] * elements[11] * elements[13] -
						 elements[8] * elements[5] * elements[15] + elements[8] * elements[7] * elements[13] +
						 elements[12] * elements[5] * elements[11] - elements[12] * elements[7] * elements[9];

	result.elements[12] = -elements[4] * elements[9] * elements[14] + elements[4] * elements[10] * elements[13] +
						  elements[8] * elements[5] * elements[14] - elements[8] * elements[6] * elements[13] -
						  elements[12] * elements[5] * elements[10] + elements[12] * elements[6] * elements[9];

	result.elements[1] = -elements[1] * elements[10] * elements[15] + elements[1] * elements[11] * elements[14] +
						 elements[9] * elements[2] * elements[15] - elements[9] * elements[3] * elements[14] -
						 elements[13] * elements[2] * elements[11] + elements[13] * elements[3] * elements[10];

	result.elements[5] = elements[0] * elements[10] * elements[15] - elements[0] * elements[11] * elements[14] -
						 elements[8] * elements[2] * elements[15] + elements[8] * elements[3] * elements[14] +
						 elements[12] * elements[2] * elements[11] - elements[12] * elements[3] * elements[10];

	result.elements[9] = -elements[0] * elements[9] * elements[15] + elements[0] * elements[11] * elements[13] +
						 elements[8] * elements[1] * elements[15] - elements[8] * elements[3] * elements[13] -
						 elements[12] * elements[1] * elements[11] + elements[12] * elements[3] * elements[9];

	result.elements[13] = elements[0] * elements[9] * elements[14] - elements[0] * elements[10] * elements[13] -
						  elements[8] * elements[1] * elements[14] + elements[8] * elements[2] * elements[13] +
						  elements[12] * elements[1] * elements[10] - elements[12] * elements[2] * elements[9];

	result.elements[2] = elements[1] * elements[6] * elements[15] - elements[1] * elements[7] * elements[14] -
						 elements[5] * elements[2] * elements[15] + elements[5] * elements[3] * elements[14] +
						 elements[13] * elements[2] * elements[7] - elements[13] * elements[3] * elements[6];

	result.elements[6] = -elements[0] * elements[6] * elements[15] + elements[0] * elements[7] * elements[14] +
						 elements[4] * elements[2] * elements[15] - elements[4] * elements[3] * elements[14] -
						 elements[12] * elements[2] * elements[7] + elements[12] * elements[3] * elements[6];

	result.elements[10] = elements[0] * elements[5] * elements[15] - elements[0] * elements[7] * elements[13] -
						  elements[4] * elements[1] * elements[15] + elements[4] * elements[3] * elements[13] +
						  elements[12] * elements[1] * elements[7] - elements[12] * elements[3] * elements[5];

	result.elements[14] = -elements[0] * elements[5] * elements[14] + elements[0] * elements[6] * elements[13] +
						  elements[4] * elements[1] * elements[14] - elements[4] * elements[2] * elements[13] -
						  elements[12] * elements[1] * elements[6] + elements[12] * elements[2] * elements[5];

	result.elements[3] = -elements[1] * elements[6] * elements[11] + elements[1] * elements[7] * elements[10] +
						 elements[5] * elements[2] * elements[11] - elements[5] * elements[3] * elements[10] -
						 elements[9] * elements[2] * elements[7] + elements[9] * elements[3] * elements[6];

	result.elements[7] = elements[0] * elements[6] * elements[11] - elements[0] * elements[7] * elements[10] -
						 elements[4] * elements[2] * elements[11] + elements[4] * elements[3] * elements[10] +
						 elements[8] * elements[2] * elements[7] - elements[8] * elements[3] * elements[6];

	result.elements[11] = -elements[0] * elements[5] * elements[11] + elements[0] * elements[7] * elements[9] +
						  elements[4] * elements[1] * elements[11] - elements[4] * elements[3] * elements[9] -
						  elements[8] * elements[1] * elements[7] + elements[8] * elements[3] * elements[5];

	result.elements[15] = elements[0] * elements[5] * elements[10] - elements[0] * elements[6] * elements[9] -
						  elements[4] * elements[1] * elements[10] + elements[4] * elements[2] * elements[9] +
						  elements[8] * elements[1] * elements[6] - elements[8] * elements[2] * elements[5];

	const auto det = elements[0] * result.elements[0] + elements[1] * result.elements[4] +
					 elements[2] * result.elements[8] + elements[3] * result.elements[12];
	if (det == 0.f) {
		return Option<Matrix4>{};
	}
	const auto det_reciprocal = (T)1 / det;

	for (int i = 0; i < 16; ++i) {
		result.elements[i] *= det_reciprocal;
	}

	return result;
}

template <Number T>
Vector4<T> Matrix4<T>::row(usize index) const {
	OP_ASSERT(index < 4);
	switch (index) {
	case 0:
		return { x.x, y.x, z.x, w.x };
	case 1:
		return { x.y, y.y, z.y, w.y };
	case 2:
		return { x.z, y.z, z.z, w.z };
	case 3:
		return { x.w, y.w, z.w, w.w };
	}
	return {};
}

template <Number T>
Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& rhs) const {
	Vector4<T> row_x = {};
	const auto row0 = row(0);
	row_x.x = row0.dot(rhs.x);
	row_x.y = row0.dot(rhs.y);
	row_x.z = row0.dot(rhs.z);
	row_x.w = row0.dot(rhs.w);

	Vector4<T> row_y = {};
	const auto row1 = row(1);
	row_y.x = row1.dot(rhs.x);
	row_y.y = row1.dot(rhs.y);
	row_y.z = row1.dot(rhs.z);
	row_y.w = row1.dot(rhs.w);

	Vector4<T> row_z = {};
	const auto row2 = row(2);
	row_z.x = row2.dot(rhs.x);
	row_z.y = row2.dot(rhs.y);
	row_z.z = row2.dot(rhs.z);
	row_z.w = row2.dot(rhs.w);

	Vector4<T> row_w = {};
	const auto row3 = row(3);
	row_w.x = row3.dot(rhs.x);
	row_w.y = row3.dot(rhs.y);
	row_w.z = row3.dot(rhs.z);
	row_w.w = row3.dot(rhs.w);

	return Matrix4::from_rows(row_x, row_y, row_z, row_w);
}

template <Number T>
Vector4<T> Matrix4<T>::operator*(const Vector4<T>& rhs) const {
	const auto x = row(0).dot(rhs);
	const auto y = row(1).dot(rhs);
	const auto z = row(2).dot(rhs);
	const auto w = row(3).dot(rhs);

	return { x, y, z, w };
}

OP_CORE_NAMESPACE_END
