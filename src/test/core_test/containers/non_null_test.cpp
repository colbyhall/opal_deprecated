// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/non_null.h"
#include "doctest/doctest.h"

GJ_TEST_BEGIN

TEST_CASE("gj::core::NonNull") {
	using namespace gj;
	int x = 42;

	// NonNull's behave as references but are easier to work with
	NonNull<int> p_x = &x;
	CHECK(*p_x == 42);

	// They can only be initialized with a valid pointer and will throw errors
	// at compile time or runtime if a nullptr is detected.
	// NonNull<int> p_y = nullptr; causes compile error
	// NonNull<int> p_y; causes compile error

	// NonNull's can be copied
	NonNull<int> p_y = p_x;
	CHECK(*p_y == x);

	// NonNull's can be compared to each other or raw pointers
	CHECK(p_y == &x);
	CHECK(p_y == p_x);
	NonNull<void> p_v = &x;
	//	CHECK(p_v == p_y);

	// NonNull<void> has a method for static_casting to the correct type
	auto p_z = p_v.as<int>();
	CHECK(p_z == p_x);
}

GJ_TEST_END
