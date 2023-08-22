// Copyright Colby Hall. All Rights Reserved.

#include "core/math/math.h"
#include "doctest/doctest.h"

SF_TEST_BEGIN

TEST_CASE("sf::core::math") {
	SUBCASE("min/max") {
		CHECK(sf::min(5, 10) == 5);
		CHECK(sf::max(5, 10) == 10);
	}

	SUBCASE("clamp") {
		CHECK(sf::clamp(5, 0, 10) == 5);
		CHECK(sf::clamp(-1, 0, 10) == 0);
		CHECK(sf::clamp(11, 0, 10) == 10);
	}

	SUBCASE("pi/tau") {
		CHECK(sf::pi<f32> == doctest::Approx(3.14159265359f));
		CHECK(sf::tau<f32> == doctest::Approx(2 * 3.14159265359f));
	}

	SUBCASE("deg_to_rad/rad_to_deg") {
		CHECK(sf::deg_to_rad<f32> == doctest::Approx(sf::pi<f32> / 180.f));
		CHECK(sf::rad_to_deg<f32> == doctest::Approx(180.f / sf::pi<f32>));
	}

	SUBCASE("near_zero") {
		CHECK(sf::core::is_near_zero<f32>(sf::core::near_zero<f32>) == true);
		CHECK(sf::core::is_near_zero<f32>(0.0f) == true);
		CHECK(sf::core::is_near_zero<f32>(0.00000001f) == true);
		CHECK(sf::core::is_near_zero<f32>(0.0000001f) == false);
	}

	SUBCASE("abs") {
		CHECK(sf::abs(-5) == 5);
		CHECK(sf::abs(5) == 5);
	}

	SUBCASE("lerp") { CHECK(sf::lerp(0.f, 10.f, 0.5f) == 5.f); }

	SUBCASE("trigonometric functions") {
		CHECK(sf::cos(0.0) == doctest::Approx(1.0));
		CHECK(sf::sin(sf::pi<f32> / 2) == doctest::Approx(1.0));
		CHECK(sf::tan(sf::pi<f32> / 4) == doctest::Approx(1.0));
	}

	SUBCASE("inverse trigonometric functions") {
		CHECK(sf::acos(1.0) == doctest::Approx(0.0));
		CHECK(sf::asin(1.0) == doctest::Approx(sf::pi<f32> / 2));
		CHECK(sf::atan(1.0) == doctest::Approx(sf::pi<f32> / 4));
	}

	SUBCASE("sqrt") { CHECK(sf::sqrt(4.0) == doctest::Approx(2.0)); }

	SUBCASE("fmod") { CHECK(sf::fmod(10.0, 3.0) == doctest::Approx(1.0)); }
}

SF_TEST_END
