// Copyright Colby Hall. All Rights Reserved.

#include "core/math/math.h"
#include "doctest/doctest.h"

GJ_TEST_BEGIN

TEST_CASE("gj::core::math") {
	SUBCASE("min/max") {
		CHECK(gj::min(5, 10) == 5);
		CHECK(gj::max(5, 10) == 10);
	}

	SUBCASE("clamp") {
		CHECK(gj::clamp(5, 0, 10) == 5);
		CHECK(gj::clamp(-1, 0, 10) == 0);
		CHECK(gj::clamp(11, 0, 10) == 10);
	}

	SUBCASE("pi/tau") {
		CHECK(gj::pi<f32> == doctest::Approx(3.14159265359f));
		CHECK(gj::tau<f32> == doctest::Approx(2 * 3.14159265359f));
	}

	SUBCASE("deg_to_rad/rad_to_deg") {
		CHECK(gj::deg_to_rad<f32> == doctest::Approx(gj::pi<f32> / 180.f));
		CHECK(gj::rad_to_deg<f32> == doctest::Approx(180.f / gj::pi<f32>));
	}

	SUBCASE("near_zero") {
		CHECK(gj::core::is_near_zero<f32>(gj::core::near_zero<f32>) == true);
		CHECK(gj::core::is_near_zero<f32>(0.0f) == true);
		CHECK(gj::core::is_near_zero<f32>(0.00000001f) == true);
		CHECK(gj::core::is_near_zero<f32>(0.0000001f) == false);
	}

	SUBCASE("abs") {
		CHECK(gj::abs(-5) == 5);
		CHECK(gj::abs(5) == 5);
	}

	SUBCASE("lerp") { CHECK(gj::lerp(0.f, 10.f, 0.5f) == 5.f); }

	SUBCASE("trigonometric functions") {
		CHECK(gj::cos(0.0) == doctest::Approx(1.0));
		CHECK(gj::sin(gj::pi<f32> / 2) == doctest::Approx(1.0));
		CHECK(gj::tan(gj::pi<f32> / 4) == doctest::Approx(1.0));
	}

	SUBCASE("inverse trigonometric functions") {
		CHECK(gj::acos(1.0) == doctest::Approx(0.0));
		CHECK(gj::asin(1.0) == doctest::Approx(gj::pi<f32> / 2));
		CHECK(gj::atan(1.0) == doctest::Approx(gj::pi<f32> / 4));
	}

	SUBCASE("sqrt") { CHECK(gj::sqrt(4.0) == doctest::Approx(2.0)); }

	SUBCASE("fmod") { CHECK(gj::fmod(10.0, 3.0) == doctest::Approx(1.0)); }
}

GJ_TEST_END
