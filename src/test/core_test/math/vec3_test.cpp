// Copyright Colby Hall. All Rights Reserved.

#include "core/math/vector3.h"
#include "doctest/doctest.h"

OP_TEST_BEGIN

TEST_CASE("op::core::Vector3") {
	SUBCASE("Constructors") {
		const Vector3<f64> default_constructed;
		CHECK(default_constructed.x == doctest::Approx(0.0));
		CHECK(default_constructed.y == doctest::Approx(0.0));
		CHECK(default_constructed.z == doctest::Approx(0.0));

		const Vector3<f64> one_param = 45.0;
		CHECK(one_param.x == doctest::Approx(45.0));
		CHECK(one_param.y == doctest::Approx(45.0));
		CHECK(one_param.z == doctest::Approx(45.0));

		const Vector3<f64> two_param = { Vector2<f64>{ 1.0, 2.0 }, 3.0 };
		CHECK(two_param.x == doctest::Approx(1.0));
		CHECK(two_param.y == doctest::Approx(2.0));
		CHECK(two_param.z == doctest::Approx(3.0));

		const Vector3<f64> three_param = { 45.0, 0.0, 100.0 };
		CHECK(three_param.x == doctest::Approx(45.0));
		CHECK(three_param.y == doctest::Approx(0.0));
		CHECK(three_param.z == doctest::Approx(100.0));
	}

	SUBCASE("Addition") {
		const Vector3<f64> a = { 1.0, 2.0, 3.0 };
		const Vector3<f64> b = { 5.0, 5.0, 6.0 };
		const auto c = a + b;
		CHECK(c.x == doctest::Approx(6.0));
		CHECK(c.y == doctest::Approx(7.0));
		CHECK(c.z == doctest::Approx(9.0));
	}

	SUBCASE("Subtraction") {
		const Vector3<f64> a = { 1.0, 2.0, 3.0 };
		const Vector3<f64> b = { 5.0, 5.0, 6.0 };
		const auto c = b - a;
		CHECK(c.x == doctest::Approx(4.0));
		CHECK(c.y == doctest::Approx(3.0));
		CHECK(c.z == doctest::Approx(3.0));
	}

	SUBCASE("Multiplication") {
		const Vector3<f64> a = { 1.0, 2.0, 3.0 };
		const Vector3<f64> b = { 5.0, 5.0, 6.0 };
		const auto c = a * b;
		CHECK(c.x == doctest::Approx(5.0));
		CHECK(c.y == doctest::Approx(10.0));
		CHECK(c.z == doctest::Approx(18.0));
	}

	SUBCASE("Division") {
		const Vector3<f64> a = { 1.0, 2.0, 3.0 };
		const Vector3<f64> b = { 5.0, 5.0, 6.0 };
		const auto c = b / a;
		CHECK(c.x == doctest::Approx(5.0));
		CHECK(c.y == doctest::Approx(2.5));
		CHECK(c.z == doctest::Approx(2.0));
	}

	SUBCASE("Dot product") {
		const Vector3<f64> a = { 1.0, 2.0, 3.0 };
		const Vector3<f64> b = { 5.0, 5.0, 6.0 };
		const auto c = b.dot(a);
		CHECK(c == doctest::Approx(5.0 + 10.0 + 18.0));
	}

	SUBCASE("Cross product") {
		const Vector3<f64> a = { 1.0, 0.0, 0.0 };
		const Vector3<f64> b = { 0.0, 1.0, 0.0 };
		const auto c = a.cross(b);
		CHECK(c.x == doctest::Approx(0.0));
		CHECK(c.y == doctest::Approx(0.0));
		CHECK(c.z == doctest::Approx(1.0));
	}

	SUBCASE("Length square") {
		const Vector3<f64> v = { 3.0, 4.0, 5.0 };
		const auto len_sq = v.len_sq();
		CHECK(len_sq == doctest::Approx(9.0 + 16.0 + 25.0));
	}

	SUBCASE("Length") {
		const Vector3<f64> v = { 3.0, 4.0, 5.0 };
		const auto len = v.len();
		CHECK(len == doctest::Approx(op::sqrt(9.0 + 16.0 + 25.0)));
	}

	SUBCASE("Normalized") {
		const Vector3<f64> a = { 1.0, 0.0, 0.0 };
		const auto normalized = a.normalized().unwrap();
		CHECK(normalized.x == doctest::Approx(1.0));
		CHECK(normalized.y == doctest::Approx(0.0));
		CHECK(normalized.z == doctest::Approx(0.0));
	}
}

OP_TEST_END
