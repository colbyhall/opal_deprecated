// Copyright Colby Hall. All Rights Reserved.

#include "core/math/math.h"
#include "doctest/doctest.h"

OP_TEST_BEGIN

TEST_CASE("op::core::math") {
	SUBCASE("min/max") {
		CHECK(op::min(5, 10) == 5);
		CHECK(op::max(5, 10) == 10);
	}

	SUBCASE("clamp") {
		CHECK(op::clamp(5, 0, 10) == 5);
		CHECK(op::clamp(-1, 0, 10) == 0);
		CHECK(op::clamp(11, 0, 10) == 10);
	}

	SUBCASE("pi/tau") {
		CHECK(op::pi<f32> == doctest::Approx(3.14159265359f));
		CHECK(op::tau<f32> == doctest::Approx(2 * 3.14159265359f));
	}

	SUBCASE("deg_to_rad/rad_to_deg") {
		CHECK(op::deg_to_rad<f32> == doctest::Approx(op::pi<f32> / 180.f));
		CHECK(op::rad_to_deg<f32> == doctest::Approx(180.f / op::pi<f32>));
	}

	SUBCASE("near_zero") {
		CHECK(op::core::is_near_zero<f32>(op::core::near_zero<f32>) == true);
		CHECK(op::core::is_near_zero<f32>(0.0f) == true);
		CHECK(op::core::is_near_zero<f32>(0.00000001f) == true);
		CHECK(op::core::is_near_zero<f32>(0.0000001f) == false);
	}

	SUBCASE("abs") {
		CHECK(op::abs(-5) == 5);
		CHECK(op::abs(5) == 5);
	}

	SUBCASE("lerp") { CHECK(op::lerp(0.f, 10.f, 0.5f) == 5.f); }

	SUBCASE("trigonometric functions") {
		CHECK(op::cos(0.0) == doctest::Approx(1.0));
		CHECK(op::sin(op::pi<f32> / 2) == doctest::Approx(1.0));
		CHECK(op::tan(op::pi<f32> / 4) == doctest::Approx(1.0));
	}

	SUBCASE("inverse trigonometric functions") {
		CHECK(op::acos(1.0) == doctest::Approx(0.0));
		CHECK(op::asin(1.0) == doctest::Approx(op::pi<f32> / 2));
		CHECK(op::atan(1.0) == doctest::Approx(op::pi<f32> / 4));
	}

	SUBCASE("sqrt") { CHECK(op::sqrt(4.0) == doctest::Approx(2.0)); }

	SUBCASE("fmod") { CHECK(op::fmod(10.0, 3.0) == doctest::Approx(1.0)); }
}

OP_TEST_END
