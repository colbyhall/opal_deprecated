// Copyright Colby Hall. All Rights Reserved.

#include "core/math/math.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

TEST_CASE("eu::core::math") {
	using namespace eu;

	SUBCASE("min/max") {
		CHECK(eu::min(5, 10) == 5);
		CHECK(eu::max(5, 10) == 10);
	}

	SUBCASE("clamp") {
		CHECK(eu::clamp(5, 0, 10) == 5);
		CHECK(eu::clamp(-1, 0, 10) == 0);
		CHECK(eu::clamp(11, 0, 10) == 10);
	}

	SUBCASE("pi/tau") {
		CHECK(eu::pi<f32> == doctest::Approx(3.14159265359f));
		CHECK(eu::tau<f32> == doctest::Approx(2 * 3.14159265359f));
	}

	SUBCASE("deg_to_rad/rad_to_deg") {
		CHECK(eu::deg_to_rad<f32> == doctest::Approx(eu::pi<f32> / 180.f));
		CHECK(eu::rad_to_deg<f32> == doctest::Approx(180.f / eu::pi<f32>));
	}

	SUBCASE("near_zero") {
		CHECK(eu::core::is_near_zero<f32>(eu::core::near_zero<f32>) == true);
		CHECK(eu::core::is_near_zero<f32>(0.0f) == true);
		CHECK(eu::core::is_near_zero<f32>(0.00000001f) == true);
		CHECK(eu::core::is_near_zero<f32>(0.0000001f) == false);
	}

	SUBCASE("abs") {
		CHECK(eu::abs(-5) == 5);
		CHECK(eu::abs(5) == 5);
	}

	SUBCASE("lerp") { CHECK(eu::lerp(0.f, 10.f, 0.5f) == 5.f); }

	SUBCASE("trigonometric functions") {
		CHECK(eu::cos(0.0) == doctest::Approx(1.0));
		CHECK(eu::sin(eu::pi<f32> / 2) == doctest::Approx(1.0));
		CHECK(eu::tan(eu::pi<f32> / 4) == doctest::Approx(1.0));
	}

	SUBCASE("inverse trigonometric functions") {
		CHECK(eu::acos(1.0) == doctest::Approx(0.0));
		CHECK(eu::asin(1.0) == doctest::Approx(eu::pi<f32> / 2));
		CHECK(eu::atan(1.0) == doctest::Approx(eu::pi<f32> / 4));
	}

	SUBCASE("sqrt") { CHECK(eu::sqrt(4.0) == doctest::Approx(2.0)); }

	SUBCASE("fmod") { CHECK(eu::fmod(10.0, 3.0) == doctest::Approx(1.0)); }
}

EU_TEST_END
