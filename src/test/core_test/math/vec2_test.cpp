// Copyright Colby Hall. All Rights Reserved.

#include "core/math/vec2.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

TEST_CASE("eu::core::Vec2") {
	using namespace eu;

	SUBCASE("Constructors") {
		const Vec2f64 default_constructed;
		CHECK(default_constructed.x == doctest::Approx(0.0));
		CHECK(default_constructed.y == doctest::Approx(0.0));

		const Vec2f64 one_param = 45.0;
		CHECK(one_param.x == doctest::Approx(45.0));
		CHECK(one_param.y == doctest::Approx(45.0));

		const Vec2f64 two_param = { 45.0, 0.0 };
		CHECK(two_param.x == doctest::Approx(45.0));
		CHECK(two_param.y == doctest::Approx(0.0));
	}

	SUBCASE("Addition") {
		const Vec2f64 a = { 1.0, 2.0 };
		const Vec2f64 b = { 3.0, 4.0 };
		const auto c = a + b;
		CHECK(c.x == doctest::Approx(4.0));
		CHECK(c.y == doctest::Approx(6.0));
	}

	SUBCASE("Subtraction") {
		const Vec2f64 a = { 1.0, 2.0 };
		const Vec2f64 b = { 3.0, 4.0 };
		const auto c = b - a;
		CHECK(c.x == doctest::Approx(2.0));
		CHECK(c.y == doctest::Approx(2.0));
	}

	SUBCASE("Multiplication") {
		const Vec2f64 a = { 1.0, 2.0 };
		const Vec2f64 b = { 3.0, 4.0 };
		const auto c = a * b;
		CHECK(c.x == doctest::Approx(3.0));
		CHECK(c.y == doctest::Approx(8.0));
	}

	SUBCASE("Division") {
		const Vec2f64 a = { 1.0, 2.0 };
		const Vec2f64 b = { 3.0, 4.0 };
		const auto c = b / a;
		CHECK(c.x == doctest::Approx(3.0));
		CHECK(c.y == doctest::Approx(2.0));
	}

	SUBCASE("Dot product") {
		const Vec2f64 a = { 1.0, 2.0 };
		const Vec2f64 b = { 3.0, 4.0 };
		const auto c = b.dot(a);
		CHECK(c == doctest::Approx(11.0));
	}

	SUBCASE("Length square") {
		const Vec2f64 v = { 3.0, 4.0 };
		const auto len_sq = v.len_sq();
		CHECK(len_sq == doctest::Approx(25.0));
	}

	SUBCASE("Length") {
		const Vec2f64 v = { 3.0, 4.0 };
		const auto len = v.len();
		CHECK(len == doctest::Approx(5.0));
	}

	SUBCASE("Normalized") {
		const Vec2f64 a = { 1.0, 0.0 };
		const auto normalized = a.normalized().unwrap();
		CHECK(normalized.x == doctest::Approx(1.0));
		CHECK(normalized.y == doctest::Approx(0.0));
	}
}

EU_TEST_END
